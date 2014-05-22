//===- Scope.cpp - Lexical scope information --------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Scope class, which is used for recording
// information about a lexical scope.
//
//===----------------------------------------------------------------------===//

#include "clang/Sema/Scope.h"
#include "clang/AST/Decl.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

void Scope::Init(Scope *parent, unsigned flags) {
  AnyParent = parent;
  Flags = flags;

  if (parent && !(flags & FnScope)) {
    BreakParent    = parent->BreakParent;
    ContinueParent = parent->ContinueParent;
  } else {
    // Control scopes do not contain the contents of nested function scopes for
    // control flow purposes.
    BreakParent = ContinueParent = 0;
  }

  if (parent) {
    Depth = parent->Depth + 1;
    PrototypeDepth = parent->PrototypeDepth;
    PrototypeIndex = 0;
    FnParent       = parent->FnParent;
    BlockParent    = parent->BlockParent;
    TemplateParamParent = parent->TemplateParamParent;
    MSLocalManglingParent = parent->MSLocalManglingParent;
  } else {
    Depth = 0;
    PrototypeDepth = 0;
    PrototypeIndex = 0;
    MSLocalManglingParent = FnParent = BlockParent = 0;
    TemplateParamParent = 0;
    MSLocalManglingNumber = 1;
  }

  // If this scope is a function or contains breaks/continues, remember it.
  if (flags & FnScope)            FnParent = this;
  // The MS mangler uses the number of scopes that can hold declarations as
  // part of an external name.
  if (Flags & (ClassScope | FnScope)) {
    MSLocalManglingNumber = getMSLocalManglingNumber();
    MSLocalManglingParent = this;
  }
  if (flags & BreakScope)         BreakParent = this;
  if (flags & ContinueScope)      ContinueParent = this;
  if (flags & BlockScope)         BlockParent = this;
  if (flags & TemplateParamScope) TemplateParamParent = this;

  // If this is a prototype scope, record that.
  if (flags & FunctionPrototypeScope) PrototypeDepth++;
  if (flags & DeclScope) {
    if (flags & FunctionPrototypeScope)
      ; // Prototype scopes are uninteresting.
    else if ((flags & ClassScope) && getParent()->isClassScope())
      ; // Nested class scopes aren't ambiguous.
    else if ((flags & ClassScope) && getParent()->getFlags() == DeclScope)
      ; // Classes inside of namespaces aren't ambiguous.
    else
      incrementMSLocalManglingNumber();
  }

  DeclsInScope.clear();
  PrefixesInScope.clear(); // eero namespace prefixes
  UsingDirectives.clear();
  Entity = 0;
  ErrorTrap.reset();
  NRVO.setPointerAndInt(nullptr, 0);
}

bool Scope::containedInPrototypeScope() const {
  const Scope *S = this;
  while (S) {
    if (S->isFunctionPrototypeScope())
      return true;
    S = S->getParent();
  }
  return false;
}

void Scope::AddFlags(unsigned FlagsToSet) {
  assert((FlagsToSet & ~(BreakScope | ContinueScope)) == 0 &&
         "Unsupported scope flags");
  if (FlagsToSet & BreakScope) {
    assert((Flags & BreakScope) == 0 && "Already set");
    BreakParent = this;
  }
  if (FlagsToSet & ContinueScope) {
    assert((Flags & ContinueScope) == 0 && "Already set");
    ContinueParent = this;
  }
  Flags |= FlagsToSet;
}

void Scope::mergeNRVOIntoParent() {
  if (VarDecl *Candidate = NRVO.getPointer()) {
    if (isDeclScope(Candidate))
      Candidate->setNRVOVariable(true);
  }

  if (getEntity())
    return;

  if (NRVO.getInt())
    getParent()->setNoNRVO();
  else if (NRVO.getPointer())
    getParent()->addNRVOCandidate(NRVO.getPointer());
}

void Scope::dump() const { dumpImpl(llvm::errs()); }

void Scope::dumpImpl(raw_ostream &OS) const {
  unsigned Flags = getFlags();
  bool HasFlags = Flags != 0;

  if (HasFlags)
    OS << "Flags: ";

  while (Flags) {
    if (Flags & FnScope) {
      OS << "FnScope";
      Flags &= ~FnScope;
    } else if (Flags & BreakScope) {
      OS << "BreakScope";
      Flags &= ~BreakScope;
    } else if (Flags & ContinueScope) {
      OS << "ContinueScope";
      Flags &= ~ContinueScope;
    } else if (Flags & DeclScope) {
      OS << "DeclScope";
      Flags &= ~DeclScope;
    } else if (Flags & ControlScope) {
      OS << "ControlScope";
      Flags &= ~ControlScope;
    } else if (Flags & ClassScope) {
      OS << "ClassScope";
      Flags &= ~ClassScope;
    } else if (Flags & BlockScope) {
      OS << "BlockScope";
      Flags &= ~BlockScope;
    } else if (Flags & TemplateParamScope) {
      OS << "TemplateParamScope";
      Flags &= ~TemplateParamScope;
    } else if (Flags & FunctionPrototypeScope) {
      OS << "FunctionPrototypeScope";
      Flags &= ~FunctionPrototypeScope;
    } else if (Flags & FunctionDeclarationScope) {
      OS << "FunctionDeclarationScope";
      Flags &= ~FunctionDeclarationScope;
    } else if (Flags & AtCatchScope) {
      OS << "AtCatchScope";
      Flags &= ~AtCatchScope;
    } else if (Flags & ObjCMethodScope) {
      OS << "ObjCMethodScope";
      Flags &= ~ObjCMethodScope;
    } else if (Flags & SwitchScope) {
      OS << "SwitchScope";
      Flags &= ~SwitchScope;
    } else if (Flags & TryScope) {
      OS << "TryScope";
      Flags &= ~TryScope;
    } else if (Flags & FnTryCatchScope) {
      OS << "FnTryCatchScope";
      Flags &= ~FnTryCatchScope;
    } else if (Flags & OpenMPDirectiveScope) {
      OS << "OpenMPDirectiveScope";
      Flags &= ~OpenMPDirectiveScope;
    }

    if (Flags)
      OS << " | ";
  }
  if (HasFlags)
    OS << '\n';

  if (const Scope *Parent = getParent())
    OS << "Parent: (clang::Scope*)" << Parent << '\n';

  OS << "Depth: " << Depth << '\n';
  OS << "MSLocalManglingNumber: " << getMSLocalManglingNumber() << '\n';
  if (const DeclContext *DC = getEntity())
    OS << "Entity : (clang::DeclContext*)" << DC << '\n';

  if (NRVO.getInt())
    OS << "NRVO not allowed";
  else if (NRVO.getPointer())
    OS << "NRVO candidate : (clang::VarDecl*)" << NRVO.getPointer() << '\n';
}
