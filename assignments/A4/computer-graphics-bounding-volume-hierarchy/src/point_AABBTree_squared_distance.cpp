#include "point_AABBTree_squared_distance.h"

#include <queue>  // std::priority_queue

#include "CloudPoint.h"

typedef std::pair<double, std::shared_ptr<Object>> pair;

bool point_AABBTree_squared_distance(const Eigen::RowVector3d& query, const std::shared_ptr<AABBTree>& root,
    const double min_sqrd, const double max_sqrd, double& sqrd, std::shared_ptr<Object>& descendant) {

    std::priority_queue<pair, std::vector<pair>, std::greater<pair>> minPriorityQueue;

    pair element;
    element = std::make_pair(point_box_squared_distance(query, root->box), root);
    minPriorityQueue.push(element);

    sqrd = std::numeric_limits<double>::infinity();

    while (!minPriorityQueue.empty()) {
        pair curElement = minPriorityQueue.top();
        minPriorityQueue.pop();
        if (curElement.first < sqrd) {
            // curElement.second could be an AABB tree or a CloudPoint or a MeshTriangle
            std::shared_ptr<AABBTree> node = std::dynamic_pointer_cast<AABBTree>(curElement.second);
            if (node) {
                pair elementLeft;
                if (node->left != nullptr) {
                    elementLeft = std::make_pair(point_box_squared_distance(query, node->left->box), node->left);
                    minPriorityQueue.push(elementLeft);
                }
                pair elementRight;
                if (node->right != nullptr) {
                    elementRight = std::make_pair(point_box_squared_distance(query, node->right->box), node->right);
                    minPriorityQueue.push(elementRight);
                }
            } else {
                // This a leaf node (i.e. either a CloudPoint or a MeshTriangle)
                if (curElement.first < sqrd) {
                    sqrd = curElement.first;
                    descendant = curElement.second;
                }
            }
        }
    }

    return sqrd >= min_sqrd && sqrd <= max_sqrd;
}
