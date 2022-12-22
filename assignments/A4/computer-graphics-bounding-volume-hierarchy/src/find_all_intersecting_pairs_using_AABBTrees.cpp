#include "find_all_intersecting_pairs_using_AABBTrees.h"
#include "box_box_intersect.h"
#include "MeshTriangle.h"
#include "triangle_triangle_intersection.h"
// Hint: use a list as a queue
#include <list> 
#include <iostream>

void find_all_intersecting_pairs_using_AABBTrees(
  const std::shared_ptr<AABBTree> & rootA,
  const std::shared_ptr<AABBTree> & rootB,
  std::vector<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object> > > & 
    leaf_pairs)
{
    // We want to find the list of all intersecting triangles
    std::list<std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>>> queue;

    if (box_box_intersect(rootA->box, rootB->box)) {
        queue.push_back(std::make_pair(rootA, rootB));
    }

    while (!queue.empty()) {
        std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>> pair = queue.front();
        std::shared_ptr<MeshTriangle> triangleA = std::dynamic_pointer_cast<MeshTriangle>(pair.first);
        std::shared_ptr<MeshTriangle> triangleB = std::dynamic_pointer_cast<MeshTriangle>(pair.second);
        queue.pop_front();

        if (triangleA && triangleB) {
            // Both are leaves (i.e. A Triangle or a CloudPoint).
            // In this case, we know they are triangles.
            leaf_pairs.push_back(std::make_pair(pair.first, pair.second));
        } else {
            std::shared_ptr<AABBTree> A = std::dynamic_pointer_cast<AABBTree>(pair.first);
            std::shared_ptr<AABBTree> B = std::dynamic_pointer_cast<AABBTree>(pair.second);
            if (triangleA && B) {
                // Node triangleA is a leaf, but B is a AABBTrees.
                if (B->left != nullptr && box_box_intersect(triangleA->box, B->left->box)) {
                    queue.push_back(std::make_pair(triangleA, B->left));
                }
                if (B->right != nullptr && box_box_intersect(triangleA->box, B->right->box)) {
                    queue.push_back(std::make_pair(triangleA, B->right));
                }
            }
            else if (A && triangleB) {
                // Node B is a leaf, but A is a AABBTrees.
                if (A->left != nullptr && box_box_intersect(triangleB->box, A->left->box)) {
                    queue.push_back(std::make_pair(A->left, triangleB));
                }
                if (A->right != nullptr && box_box_intersect(triangleB->box, A->right->box)) {
                    queue.push_back(std::make_pair(A->right, triangleB));
                }
            }
            else {
                // Both nodes A and B are AABBTrees.
                if (A->left != nullptr && B->left != nullptr && box_box_intersect(A->left->box, B->left->box)) {
                    queue.push_back(std::make_pair(A->left, B->left));
                }
                if (A->left != nullptr && B->right != nullptr && box_box_intersect(A->left->box, B->right->box)) {
                    queue.push_back(std::make_pair(A->left, B->right));
                }
                if (A->right != nullptr && B->left != nullptr && box_box_intersect(A->right->box, B->left->box)) {
                    queue.push_back(std::make_pair(A->right, B->left));
                }
                if (A->right != nullptr && B->right != nullptr && box_box_intersect(A->right->box, B->right->box)) {
                    queue.push_back(std::make_pair(A->right, B->right));
                }
            }
        }
    }
}
