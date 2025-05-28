#pragma once

#include "../generated/ifccBaseVisitor.h"
#include "VarIndex.h"

#include <iostream>
#include <string>

class CodeVarVisitor : public ifccBaseVisitor {

private:
  VarIndex varIndex;

  bool deadlyError = false;

  void printErrorIfUsedVariableNotDefined(const std::string &varName) {
    if (!varIndex.hasVariable(varName)) {
      std::cerr << "Error: Variable '" << varName << "' is not defined.\n";
      this->deadlyError = true;
    }
  }

public:
  virtual std::any visitProg(ifccParser::ProgContext *ctx) override {
    // Loop through all core in context
    for (ifccParser::CoreContext *coreCtx : ctx->core()) {
      this->visit(coreCtx);
    }

    this->visit(ctx->return_stmt());
    return 0;
  }

  virtual std::any
  visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override {
    // Check that the variable is defined
    if (ctx->VARIABLE() != nullptr) {
      std::string varName = ctx->VARIABLE()->getText();
      this->printErrorIfUsedVariableNotDefined(varName);
    }
    return 0;
  }

  virtual std::any visitCore(ifccParser::CoreContext *ctx) override {
    // Visit all declarations in the core
    if (ctx->declaration() != nullptr) {
      this->visit(ctx->declaration());
    }
    // Visit all assignments in the core
    if (ctx->assignment() != nullptr) {
      this->visit(ctx->assignment());
    }
    return 0;
  }

  virtual std::any
  visitDeclaration(ifccParser::DeclarationContext *ctx) override {
    /*
      declaration: INT VARIABLE '=' CONST ';'
              | INT VARIABLE '=' VARIABLE ';'
              | INT VARIABLE ';'
              ;
    */

    // Check if the first Variable name has not been used before
    if (ctx->VARIABLE().size() > 0 && ctx->VARIABLE(0) != nullptr) {
      std::string varName = ctx->VARIABLE(0)->getText();
      // Check if the variable already exists
      if (varIndex.hasVariable(varName)) {
        std::cerr << "Error: Variable '" << varName
                  << "' is already declared.\n";
      } else {
        // Set the variable in the index
        varIndex.setVariable(varName);
        std::cout << "Declared variable: " << varName << std::endl;
      }
    }

    // check if the second variale name does exist
    if (ctx->VARIABLE().size() > 1 && ctx->VARIABLE(1) != nullptr) {
      std::string varName = ctx->VARIABLE(1)->getText();
      this->printErrorIfUsedVariableNotDefined(varName);
    }

    return 0;
  }

  virtual std::any
  visitAssignment(ifccParser::AssignmentContext *ctx) override {
    /*
    assignment: VARIABLE '=' CONST ';'
            | VARIABLE '=' VARIABLE ';'

    */

    // Check if both variables are defined
    if (ctx->VARIABLE().size() > 0 && ctx->VARIABLE(0) != nullptr) {
      std::string varName = ctx->VARIABLE(0)->getText();
      this->printErrorIfUsedVariableNotDefined(varName);
    }

    if (ctx->VARIABLE().size() > 1 && ctx->VARIABLE(1) != nullptr) {
      std::string varName = ctx->VARIABLE(1)->getText();
      this->printErrorIfUsedVariableNotDefined(varName);
    }

    return 0;
  }
};
