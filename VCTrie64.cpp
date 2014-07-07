#include "VCTrie64.hpp"

#include <iostream>

void VCTrie64::insert(uint64_t id, uint64_t value){
    uint8_t splitPos;
    uint8_t splitBit;
    uint64_t* prev;

    if (size+2 > capacity){
        capacity += NEXT_SIZE;
        nodes = (Node*) realloc(nodes, sizeof(Node)*capacity);
        queue = (QNode*) realloc(queue, sizeof(QNode)*capacity*2);
    }

    uint64_t newNodePos = size;
    nodes[size++] = Node(id,value);

    if (newNodePos == 0) return; // It is the root

    Node* node = &(nodes[rootPos]);
    prev = &rootPos;
    splitPos = bitScanForward(value ^ node->value);
    while(!node->isLeaf && splitPos >= node->splitPos){
        splitBit = testBit(value, node->splitPos);
        prev = &(node->childPos[splitBit]);
        node = &(nodes[node->childPos[splitBit]]);
        splitPos = bitScanForward(value ^ node->value);
    }

    if (node->isLeaf && node->value == value) // we found a duplicate
            node->next = newNodePos;
    else{
        if (testBit(value, splitPos))
            nodes[size] = Node(splitPos, value, *prev, newNodePos); // There is a 1, newNode is rightChild
        else
            nodes[size] = Node(splitPos, node->value, newNodePos, *prev); // There is a 0, newNode is leftChild

        *prev = size; // The previous reference is now the new tree node
        size++;
   }
}

void VCTrie64::search1(uint64_t value, uint8_t r, VCTrie64::ReportFunction* func, void* func_args){
    if (size == 0) return;

    uint64_t count = 0;

    queue[count].pos = rootPos;
    queue[count++].r = r;
    for (uint64_t i = 0; i < count; ++i){
        if (queue[i].r < 0) continue;

        Node& node = nodes[queue[i].pos];

        if (node.isLeaf){
            uint8_t diff = popCount64(value^node.value);
            if (diff <= r)  _reportLeaf(node, diff, func, func_args);
        }
        else{
            uint8_t splitPos = testBit(value, node.splitPos);

            queue[count].pos = node.childPos[splitPos];
            queue[count++].r = queue[i].r;
            queue[count].pos = node.childPos[1-splitPos];
            queue[count++].r = queue[i].r - 1;
        }
    }
}

void VCTrie64::search2(uint64_t value, uint8_t r, VCTrie64::ReportFunction* func, void* func_args){
    if (size == 0) return;

    uint8_t diff;
    uint64_t count = 0;

    queue[count++].pos = rootPos;
    for (uint64_t i = 0; i < count; ++i){
        Node& node = nodes[queue[i].pos];
        diff = popCountUpToBit(value ^ node.value, node.splitPos); // We count all bits up to splitPos position

        if (diff > r) continue;

        if (node.isLeaf)
            _reportLeaf(node, diff, func, func_args);
        else
        {
            queue[count++].pos = node.childPos[LEFT_CHILD];
            queue[count++].pos = node.childPos[RIGHT_CHILD];
        }
    }
}

void VCTrie64::search3(uint64_t value, uint8_t r, VCTrie64::ReportFunction* func, void* func_args){
    if (size == 0) return;

    uint8_t diff;
    uint64_t count = 0;

    queue[count++].pos = rootPos;
    for (uint64_t i = 0; i < count; ++i){
        Node& node = nodes[queue[i].pos];
        diff = popCountUpToBit(value ^ node.value, node.splitPos); // We count all bits up to splitPos position

        if (diff == r) // Optimization
        {
            while(!node.isLeaf && diff <=r){
                node = nodes[node.childPos[testBit(value, node.splitPos)]];
                diff = popCountUpToBit(value ^ node.value, node.splitPos);
            }
        }
        if (diff > r) continue;

        if (node.isLeaf)
            _reportLeaf(node, diff, func, func_args);
        else
        {
            queue[count++].pos = node.childPos[LEFT_CHILD];
            queue[count++].pos = node.childPos[RIGHT_CHILD];
        }
    }
}
