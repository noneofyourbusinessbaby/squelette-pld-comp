#pragma once

#include "../generated/ifccBaseVisitor.h"
#include "VarIndex.h"

#include <iostream>
#include <string>

#include <any>

class VariableVisitor : public ifccBaseVisitor
{

private:
    VarIndex varIndex;

    bool deadlyError = false;


    void printErrorIfUsedVariableNotDefined(const std::string &varName, bool use = false)
    {
        if (!varIndex.hasVariable(varName))
        {
            std::cerr << "Error: Variable '" << varName << "' is not defined.\n";
            this->deadlyError = true;
            return;
        }

        if (use)
        {
            // Increment the usage count of the variable
            varIndex.incrementOccurrences(varName);
            std::cout << "Used variable: " << varName << std::endl;
        }
    }

    void printErrorIfVariableAlreadyDeclared(const std::string &varName)
    {
        if (varIndex.hasVariable(varName))
        {
            std::cerr << "Error: Variable '" << varName
                      << "' is already declared.\n";
        }
        else
        {
            // Set the variable in the index
            varIndex.setVariable(varName);
            std::cout << "Declared variable: " << varName << std::endl;
        }
    }

public:

    VarIndex*  getVarIndex()
    {
        return &varIndex;
    }
    
    virtual std::any visitProg(ifccParser::ProgContext *ctx)
    {
        // Loop through all core in context
        for (ifccParser::CoreContext *coreCtx : ctx->core())
        {
            this->visit(coreCtx);
        }
        this->visit(ctx->return_stmt());
        return 0;
    }

    virtual std::any visitReturn_const(ifccParser::Return_constContext *ctx)
    {
        return 0;
    }

    virtual std::any visitReturn_var(ifccParser::Return_varContext *ctx)
    {
        std::string varName = ctx->VARIABLE()->getText();
        this->printErrorIfUsedVariableNotDefined(varName, true);
        // todo var is used increment
        return 0;
    }

    virtual std::any visitCore_declaration(ifccParser::Core_declarationContext *ctx)
    {
        this->visit(ctx->declaration());
        return 0;
    }

    virtual std::any visitCore_assignment(ifccParser::Core_assignmentContext *ctx)
    {
        this->visit(ctx->assignment());
        return 0;
    }

    virtual std::any visitDeclare_const(ifccParser::Declare_constContext *ctx)
    {
        std::string varName = ctx->VARIABLE()->getText();

        this->printErrorIfVariableAlreadyDeclared(varName);

        return 0;
    }

    virtual std::any visitDeclare_var(ifccParser::Declare_varContext *ctx)
    {
        std::string varName_0 = ctx->VARIABLE(0)->getText();
        this->printErrorIfVariableAlreadyDeclared(varName_0);

        // check if the second variale name does exist
        std::string varName_1 = ctx->VARIABLE(1)->getText();
        this->printErrorIfUsedVariableNotDefined(varName_1, true);

        return 0;
    }

    virtual std::any visitDeclare_empty(ifccParser::Declare_emptyContext *ctx)
    {
        std::string varName = ctx->VARIABLE()->getText();

        this->printErrorIfVariableAlreadyDeclared(varName);

        return 0;
    }

    virtual std::any visitAssign_const(ifccParser::Assign_constContext *ctx) override
    {
        std::string varName = ctx->VARIABLE()->getText();
        this->printErrorIfUsedVariableNotDefined(varName);
        return 0;
    }

    virtual std::any visitAssign_var(ifccParser::Assign_varContext *ctx) override
    {
        std::string varName_0 = ctx->VARIABLE(0)->getText();
        this->printErrorIfUsedVariableNotDefined(varName_0, false); // Warning :  assignation is not a usage

        std::string varName_1 = ctx->VARIABLE(1)->getText();
        this->printErrorIfUsedVariableNotDefined(varName_1, true);

        return 0;
    }
};