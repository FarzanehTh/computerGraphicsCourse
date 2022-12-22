#include "AABBTree.h"

// See AABBTree.h for API
bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const 
{
    t = 0;
    if (this == nullptr || !ray_intersect_box(ray, this->box, min_t, max_t)) {
        return false;
    }

    double tLeft;
    double tRight;
    bool hitsLeftSubtree = this->left->ray_intersect(ray, min_t, max_t, tLeft, descendant);
    bool hitsRightSubtree = this->right->ray_intersect(ray, min_t, max_t, tRight, descendant);

    if (!hitsLeftSubtree && !hitsRightSubtree) {
        return false;
    }

    if (hitsLeftSubtree && hitsRightSubtree) {
        t = std::min(tLeft, tRight);
    } else if (hitsLeftSubtree) {
        t = tLeft;
    } else {
        t = tRight;
    }

    return true;
}

