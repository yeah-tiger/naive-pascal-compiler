//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_H
#define NAIVE_PASCAL_COMPILER_AST_H


class AbstractNode
{
public:
    virtual ~AbstractNode() = default;

    virtual void codegen() = 0;
};


#endif //NAIVE_PASCAL_COMPILER_AST_H
