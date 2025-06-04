#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "VarIndex.h"

class CodeGenVisitor : public ifccBaseVisitor
{
private:
        VarIndex varIndex;

public:
        void setVarIndex(const VarIndex &index)
        {
                this->varIndex = index;
        }

        void moveVariableFromStackToEax(int index)
        {
                std::cout << "    movl -" << index << "(%rpb), %eax\n";
        }

        void moveVariableFromEaxToStack(int index)
        {
                std::cout << "    movl %eax, -" << index << "(%rpb)\n";
        }

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx)
        {
#ifdef __APPLE__
                std::cout << ".globl _main\n";
                std::cout << " _main: \n";
                s
#else
                std::cout << ".globl main\n";
                std::cout << " main: \n";
#endif
                    for (ifccParser::CoreContext *coreCtx : ctx->core())
                {
                        this->visit(coreCtx);
                }
                this->visit(ctx->return_stmt());

                std::cout << "    ret\n";

                return 0;
        };

        virtual antlrcpp::Any visitReturn_const(ifccParser::Return_constContext *ctx)
        {
                int retval = stoi(ctx->CONST()->getText());

#ifdef __APPLE__
                std::cout << "    mov w0, #" << retval << "\n";
#else
                std::cout << "    movl $" << retval << ", %eax\n";
#endif

                return 0;
        };

        virtual std::any visitReturn_var(ifccParser::Return_varContext *ctx)
        {
                std::string varName = ctx->VARIABLE()->getText();

                // Get the index of the variable being returned
                int index = this->varIndex.getIndex(varName);
                this->moveVariableFromStackToEax(index);

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
                // Get variable related information
                std::string varName = ctx->VARIABLE()->getText();
                int index = this->varIndex.getIndex(varName);

                // Get the constant value
                int constantValue = stoi(ctx->CONST()->getText());

                std::cout << "    movl $" << constantValue << ", -" << index << "(%rpb)\n";

                return 0;
        }

        virtual std::any visitDeclare_var(ifccParser::Declare_varContext *ctx)
        {
                std::string varNameBeingAffected = ctx->VARIABLE(0)->getText();

                std::string varNameUsedForAffection = ctx->VARIABLE(1)->getText();

                // Move the value of the second variable to eax
                int indexVarNameUsedForAffection = this->varIndex.getIndex(varNameUsedForAffection);
                this->moveVariableFromStackToEax(indexVarNameUsedForAffection);

                // Than move eax to tne first variable
                int indexVarNameBeingAffected = this->varIndex.getIndex(varNameBeingAffected);
                this->moveVariableFromEaxToStack(indexVarNameBeingAffected);

                return 0;
        }

        virtual std::any visitDeclare_empty(ifccParser::Declare_emptyContext *ctx)
        {
                return 0;
        }

        virtual std::any visitAssign_const(ifccParser::Assign_constContext *ctx) override
        {
                std::string varName = ctx->VARIABLE()->getText();
                /*
                        todo implement
                */
                return 0;
        }

        virtual std::any visitAssign_var(ifccParser::Assign_varContext *ctx) override
        {
                std::string varName_0 = ctx->VARIABLE(0)->getText();

                std::string varName_1 = ctx->VARIABLE(1)->getText();

                /*
        todo implement
*/
                return 0;
        }
};

/*

Une affectation avec une constante :

movl $CONST, -INDEX(%rpb)

Une affectation avec une variable :

movl -INDEX_USED_FOR_AFFECTION(%rpb), %eax
movl %eax, -INDEX_BEING_AFFECTED(%rpb)


une décalaration sans variable/const : skip


une déclaration avec une variable ou bien une const : idem


return statement avec une variable :
movl -INDEX(%rpb), %eax

*/
