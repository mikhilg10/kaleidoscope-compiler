#ifndef KALEIDOSCOPE_CODEGEN_H
#define KALEIDOSCOPE_CODEGEN_H

#include "ast.h"

#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

#include <map>
#include <memory>
#include <string>

// Globals shared between codegen.cpp and main.cpp.
extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::Value *> NamedValues;
extern std::unique_ptr<llvm::orc::LLJIT> TheJIT;

extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
extern std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
extern std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
extern std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
extern std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
extern std::unique_ptr<llvm::StandardInstrumentations> TheSI;

extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

// LogErrorV - Helper for error handling in codegen.
llvm::Value *LogErrorV(const char *Str);

// getFunction - Look up a function name in the module, generating a new
// declaration from FunctionProtos if necessary.
llvm::Function *getFunction(const std::string &Name);

// InitializeModuleAndManagers - (Re)initializes TheContext, TheModule,
// Builder, the JIT, and the function pass manager / analysis managers.
void InitializeModuleAndManagers();

#endif // KALEIDOSCOPE_CODEGEN_H
