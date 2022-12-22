#include "AABBTree.h"
#include "MeshTriangle.h"
#include "CloudPoint.h"
#include "insert_box_into_box.h"
#include <iostream>

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object>> & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
    if (objects.size() == 0) {
        return;
    }    

    for (int i = 0; i < objects.size(); i++) {
        insert_box_into_box(objects[i]->box, this->box);
    }

    if (objects.size() == 1) {
        this->left = objects[0];
        return;
    } else if (objects.size() == 2) {
        this->left = objects[0];
        this->right = objects[1];
        return;
    } else {
    double maxVariation = -std::numeric_limits<double>::infinity();
    int maxIndex = 0;
    for (int i = 0; i < 3; i++) {
        double length = this->box.max_corner(i) - this->box.min_corner(i);
        if (maxVariation < length) {
            maxVariation = length;
            maxIndex = i;
        }
    }

    std::vector<std::shared_ptr<Object>> leftHalfVector;
    std::vector<std::shared_ptr<Object>> rightHalfVector;
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->box.center()(maxIndex) < this->box.center()(maxIndex)) {
            leftHalfVector.push_back(objects[i]);
        } else {
            rightHalfVector.push_back(objects[i]);
        }
    }

    // Make sure you divide the objects into both halves if one of them is empty
    if (leftHalfVector.size() > 0 && rightHalfVector.size() == 0) {
        int len = leftHalfVector.size();
        rightHalfVector = {leftHalfVector.begin() + len / 2, leftHalfVector.end()};
        leftHalfVector.resize(len / 2);
    } else if (rightHalfVector.size() > 0 && leftHalfVector.size() == 0) {
        int len = rightHalfVector.size();
        leftHalfVector = {rightHalfVector.begin() + len / 2, rightHalfVector.end()};
        rightHalfVector.resize(len / 2);
    }

    this->left = std::make_shared<AABBTree>(leftHalfVector, depth + 1);
    this->right = std::make_shared<AABBTree>(rightHalfVector, depth + 1);
    }
}
