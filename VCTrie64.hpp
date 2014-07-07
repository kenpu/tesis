#ifndef VCTRIE64_HPP
#define VCTRIE64_HPP

#include <cstdlib>
#include <stdint.h>

#include <iostream> // for testing

#include "bitops.hpp"

#define NO_NEXT -1L
#define LEFT_CHILD 0
#define RIGHT_CHILD 1

#define NEXT_SIZE 20

class VCTrie64
{
protected:
    struct Node{
        bool isLeaf;
        uint8_t splitPos;
        uint64_t value;
        union{
          uint64_t childPos[2];
          struct{
            uint64_t id;
            int64_t next;
          };
        };

        // Empty Constructor
        Node(){}

        // Constructor for Leaf nodes
        Node(uint64_t _id, uint64_t _value): isLeaf(true), splitPos(64), value(_value), id(_id), next(NO_NEXT){}

        // Constructor for TreeNode
        Node(uint8_t _splitPos, uint64_t _value, uint64_t leftPos, uint64_t rightPos):
            isLeaf(false), splitPos(_splitPos), value(_value){
            childPos[LEFT_CHILD] = leftPos;
            childPos[RIGHT_CHILD] = rightPos;
        }
    };

    struct QNode{
        uint64_t pos;
        int8_t r;
    };

    uint64_t rootPos;
    uint64_t capacity;
    uint64_t size;
    Node* nodes;
    QNode* queue;

public:
    typedef void (ReportFunction)(uint64_t id, uint8_t d, void* args);

    VCTrie64(uint64_t _capacity=NEXT_SIZE):rootPos(0),capacity(_capacity),size(0){
        nodes = (Node*) malloc(sizeof(Node)*capacity);
        queue = (QNode*) malloc(sizeof(QNode)*capacity*2);
    }
    virtual ~VCTrie64(){
        free(nodes);
        free(queue);
    }

    void insert(uint64_t id, uint64_t value);

    void search1(uint64_t value, uint8_t r, ReportFunction* func= NULL, void* func_args = NULL);
    void search2(uint64_t value, uint8_t r, ReportFunction* func= NULL, void* func_args = NULL);
    void search3(uint64_t value, uint8_t r, ReportFunction* func= NULL, void* func_args = NULL);

private:
    inline void _reportLeaf(Node& node, uint8_t d, ReportFunction* func, void* func_args){
        if (func != NULL){
            while (node.next != NO_NEXT){
                func(node.id, d, func_args);
                node = nodes[node.next];
            }
            func(node.id, d, func_args);
        }
    }
};

#endif // VCTRIE64_HPP
