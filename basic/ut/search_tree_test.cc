#include "basic/search_tree.h"

#include <set>
#include <vector>
#include <algorithm>
#include <iostream>

#include "gtest/gtest.h"

using std::set;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

TEST(SearchTreeTest, BinarySearchTreeBasic) {
  BinarySearchTree<int> tree;
  
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(0, tree.size());
  EXPECT_EQ(tree.end(), tree.begin());
  EXPECT_EQ(tree.find(1), tree.end());

  tree.insert(1);
  EXPECT_EQ(1, tree.size());
  EXPECT_FALSE(tree.end() == tree.begin());
  
  BinarySearchTree<int>::iterator i = tree.find(1);
  EXPECT_EQ(tree.begin(), i);
  EXPECT_EQ(1, *i);
  
  EXPECT_EQ(++i, tree.end());
  EXPECT_EQ(++i, tree.end());

  i = tree.begin();
  EXPECT_EQ(tree.begin(), i++);
  EXPECT_EQ(tree.end(), i++);

  EXPECT_EQ(tree.end(), --tree.end());
  EXPECT_EQ(tree.end(), --tree.begin());

  EXPECT_EQ(1, tree.maximum());
  EXPECT_EQ(1, tree.minimum());

  tree.clear();
}

TEST(SearchTreeTest, BinarySearchTreePermutations) {
  const int ELEMENTS_COUNT = 5;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  do {
    set<int> correct_tree;
    BinarySearchTree<int> tree;

    // check insert
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.insert(elements[i]);
      tree.insert(elements[i]);
    
      EXPECT_EQ(tree.size(), i + 1);
    
      // check min/max
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (BinarySearchTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }

      EXPECT_EQ(correct_tree_elements, tree_elements);

      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j <= i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }

    // check erase
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.erase(elements[i]);
      tree.erase(elements[i]);
    
      EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.size());
    
      // check min/max
      if (i + 1 < ELEMENTS_COUNT) {
        EXPECT_EQ(*correct_tree.begin(), *tree.begin());
        EXPECT_EQ(*correct_tree.begin(), tree.minimum());
        EXPECT_EQ(*(--correct_tree.end()), tree.maximum());
      }
    
      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (BinarySearchTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }
    
      EXPECT_EQ(correct_tree_elements, tree_elements);
    
      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j > i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }
  } while (std::next_permutation(elements.begin(), elements.end()));
}

TEST(SearchTreeTest, BinarySearchTreeRandom) {
  const int ELEMENTS_COUNT = 1000;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  std::random_shuffle(elements.begin(), elements.end());

  set<int> correct_tree;
  BinarySearchTree<int> tree;

  // check insert
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    correct_tree.insert(elements[i]);
    tree.insert(elements[i]);
    
    EXPECT_EQ(tree.size(), i + 1);
    
    // check min/max
    EXPECT_EQ(*correct_tree.begin(), *tree.begin());
    EXPECT_EQ(*correct_tree.begin(), tree.minimum());
    EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

    // check all
    vector<int> correct_tree_elements;
    for (set<int>::iterator j = correct_tree.begin();
         j != correct_tree.end();
         ++j) {
      correct_tree_elements.push_back(*j);
    }

    vector<int> tree_elements;
    for (BinarySearchTree<int>::iterator j = tree.begin();
         j != tree.end();
         ++j) {
      tree_elements.push_back(*j);
    }

    EXPECT_EQ(correct_tree_elements, tree_elements);
  }
  
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    correct_tree.erase(i);
    tree.erase(i);
    
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.size());
    
    // check min/max
    if (i + 1 < ELEMENTS_COUNT) {
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());
    }
    
    // check all
    vector<int> correct_tree_elements;
    for (set<int>::iterator j = correct_tree.begin();
         j != correct_tree.end();
         ++j) {
      correct_tree_elements.push_back(*j);
    }

    vector<int> tree_elements;
    for (BinarySearchTree<int>::iterator j = tree.begin();
         j != tree.end();
         ++j) {
      tree_elements.push_back(*j);
    }
    
    EXPECT_EQ(correct_tree_elements, tree_elements);
  }
}

TEST(SearchTreeTest, RBTreeBasic) {
  RBTree<int> tree;
  
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(0, tree.size());
  EXPECT_EQ(tree.end(), tree.begin());
  EXPECT_EQ(tree.find(1), tree.end());

  tree.insert(1);
  EXPECT_EQ(1, tree.size());
  EXPECT_FALSE(tree.end() == tree.begin());
  
  RBTree<int>::iterator i = tree.find(1);
  EXPECT_EQ(tree.begin(), i);
  EXPECT_EQ(1, *i);
  
  EXPECT_EQ(++i, tree.end());
  EXPECT_EQ(++i, tree.end());

  i = tree.begin();
  EXPECT_EQ(tree.begin(), i++);
  EXPECT_EQ(tree.end(), i++);

  EXPECT_EQ(tree.end(), --tree.end());
  EXPECT_EQ(tree.end(), --tree.begin());

  EXPECT_EQ(1, tree.maximum());
  EXPECT_EQ(1, tree.minimum());

  tree.clear();
}

TEST(SearchTreeTest, RBTreePermutations) {
  const int ELEMENTS_COUNT = 5;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  do {
    set<int> correct_tree;
    RBTree<int> tree;

    // check insert
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.insert(elements[i]);
      tree.insert(elements[i]);
    
      EXPECT_EQ(tree.size(), i + 1);
    
      // check min/max
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (RBTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }

      EXPECT_EQ(correct_tree_elements, tree_elements);

      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j <= i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }
  } while (std::next_permutation(elements.begin(), elements.end()));
}

TEST(SearchTreeTest, RBTreeMonotoneInsert) {
  const int ELEMENTS_COUNT = 100000;
  
  RBTree<int> tree;
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(i);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(i, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(ELEMENTS_COUNT - i - 1);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.minimum());
    EXPECT_EQ(ELEMENTS_COUNT - 1, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(0);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(0, tree.maximum());
  }
}


TEST(SearchTreeTest, RBTreeRandom) {
  for (int t = 0; t < 100; ++t) {
  const int ELEMENTS_COUNT = 100;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  std::random_shuffle(elements.begin(), elements.end());

  set<int> correct_tree;
  RBTree<int> tree;

  // check insert
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    correct_tree.insert(elements[i]);
    tree.insert(elements[i]);
    
    EXPECT_EQ(tree.size(), i + 1);
    
    // check min/max
    EXPECT_EQ(*correct_tree.begin(), *tree.begin());
    EXPECT_EQ(*correct_tree.begin(), tree.minimum());
    EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

    // check all
    vector<int> correct_tree_elements;
    for (set<int>::iterator j = correct_tree.begin();
         j != correct_tree.end();
         ++j) {
      correct_tree_elements.push_back(*j);
    }

    vector<int> tree_elements;
    for (RBTree<int>::iterator j = tree.begin();
         j != tree.end();
         ++j) {
      tree_elements.push_back(*j);
    }

    EXPECT_EQ(correct_tree_elements, tree_elements);
  }
  
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    correct_tree.erase(i);
    tree.erase(i);
    
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.size());
    
    // check min/max
    if (i + 1 < ELEMENTS_COUNT) {
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());
    }
    
    // check all
    vector<int> correct_tree_elements;
    for (set<int>::iterator j = correct_tree.begin();
         j != correct_tree.end();
         ++j) {
      correct_tree_elements.push_back(*j);
    }

    vector<int> tree_elements;
    for (RBTree<int>::iterator j = tree.begin();
         j != tree.end();
         ++j) {
      tree_elements.push_back(*j);
    }
    
    EXPECT_EQ(correct_tree_elements, tree_elements);
  }
  }
}


TEST(SearchTreeTest, SplayTreeBasic) {
  SplayTree<int> tree;
  
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(0, tree.size());
  EXPECT_EQ(tree.end(), tree.begin());
  EXPECT_EQ(tree.find(1), tree.end());

  tree.insert(1);
  EXPECT_EQ(1, tree.size());
  EXPECT_FALSE(tree.end() == tree.begin());
  
  SplayTree<int>::iterator i = tree.find(1);
  EXPECT_EQ(tree.begin(), i);
  EXPECT_EQ(1, *i);
  
  EXPECT_EQ(++i, tree.end());
  EXPECT_EQ(++i, tree.end());

  i = tree.begin();
  EXPECT_EQ(tree.begin(), i++);
  EXPECT_EQ(tree.end(), i++);

  EXPECT_EQ(tree.end(), --tree.end());
  EXPECT_EQ(tree.end(), --tree.begin());

  EXPECT_EQ(1, tree.maximum());
  EXPECT_EQ(1, tree.minimum());

  tree.insert(0);
  tree.insert(2);
  EXPECT_EQ(*tree.begin(), 0);
  EXPECT_EQ(tree.maximum(), 2);

  tree.clear();
}

TEST(SearchTreeTest, SplayTreePermutations) {
  const int ELEMENTS_COUNT = 5;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  do {
    set<int> correct_tree;
    SplayTree<int> tree;

    // check insert
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.insert(elements[i]);
      tree.insert(elements[i]);
    
      EXPECT_EQ(tree.size(), i + 1);
    
      // check min/max
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (SplayTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }

      EXPECT_EQ(correct_tree_elements, tree_elements);

      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j <= i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    } 
    // check erase
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.erase(elements[i]);
      tree.erase(elements[i]); 
    
      ASSERT_EQ(ELEMENTS_COUNT - i - 1, tree.size());
      
      // check min/max
      if (i + 1 < ELEMENTS_COUNT) {
        EXPECT_EQ(*correct_tree.begin(), *tree.begin());
        EXPECT_EQ(*correct_tree.begin(), tree.minimum());
        EXPECT_EQ(*(--correct_tree.end()), tree.maximum());
      }
    
      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (SplayTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }
    
      EXPECT_EQ(correct_tree_elements, tree_elements);
    
      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j > i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }
  } while (std::next_permutation(elements.begin(), elements.end()));
}

TEST(SearchTreeTest, SplayTreeMonotoneInsert) {
  const int ELEMENTS_COUNT = 100000;
  
  SplayTree<int> tree;
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(i);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(i, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(ELEMENTS_COUNT - i - 1);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.minimum());
    EXPECT_EQ(ELEMENTS_COUNT - 1, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(0);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(0, tree.maximum());
  }
}


TEST(SearchTreeTest, PseudoSplayTreeBasic) {
  PseudoSplayTree<int> tree;
  
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(0, tree.size());
  EXPECT_EQ(tree.end(), tree.begin());
  EXPECT_EQ(tree.find(1), tree.end());

  tree.insert(1);
  EXPECT_EQ(1, tree.size());
  EXPECT_FALSE(tree.end() == tree.begin());
  
  PseudoSplayTree<int>::iterator i = tree.find(1);
  EXPECT_EQ(tree.begin(), i);
  EXPECT_EQ(1, *i);
  
  EXPECT_EQ(++i, tree.end());
  EXPECT_EQ(++i, tree.end());

  i = tree.begin();
  EXPECT_EQ(tree.begin(), i++);
  EXPECT_EQ(tree.end(), i++);

  EXPECT_EQ(tree.end(), --tree.end());
  EXPECT_EQ(tree.end(), --tree.begin());

  EXPECT_EQ(1, tree.maximum());
  EXPECT_EQ(1, tree.minimum());

  tree.insert(0);
  tree.insert(2);
  EXPECT_EQ(*tree.begin(), 0);
  EXPECT_EQ(tree.maximum(), 2);

  tree.clear();
}

TEST(SearchTreeTest, PseudoSplayTreePermutations) {
  const int ELEMENTS_COUNT = 5;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  do {
    set<int> correct_tree;
    PseudoSplayTree<int> tree;

    // check insert
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.insert(elements[i]);
      tree.insert(elements[i]);
    
      EXPECT_EQ(tree.size(), i + 1);
    
      // check min/max
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (PseudoSplayTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }

      EXPECT_EQ(correct_tree_elements, tree_elements);

      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j <= i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    } 
    // check erase
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.erase(elements[i]);
      tree.erase(elements[i]); 
    
      ASSERT_EQ(ELEMENTS_COUNT - i - 1, tree.size());
      
      // check min/max
      if (i + 1 < ELEMENTS_COUNT) {
        EXPECT_EQ(*correct_tree.begin(), *tree.begin());
        EXPECT_EQ(*correct_tree.begin(), tree.minimum());
        EXPECT_EQ(*(--correct_tree.end()), tree.maximum());
      }
    
      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (PseudoSplayTree<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }
    
      EXPECT_EQ(correct_tree_elements, tree_elements);
    
      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j > i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }
  } while (std::next_permutation(elements.begin(), elements.end()));
}

TEST(SearchTreeTest, PseudoSplayTreeMonotoneInsert) {
  const int ELEMENTS_COUNT = 100000;
  
  PseudoSplayTree<int> tree;
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(i);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(i, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(ELEMENTS_COUNT - i - 1);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.minimum());
    EXPECT_EQ(ELEMENTS_COUNT - 1, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(0);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(0, tree.maximum());
  }
}

TEST(SearchTreeTest, TreapBasic) {
  Treap<int> tree;
  
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(0, tree.size());
  EXPECT_EQ(tree.end(), tree.begin());
  EXPECT_EQ(tree.find(1), tree.end());

  tree.insert(1);
  EXPECT_EQ(1, tree.size());
  EXPECT_FALSE(tree.end() == tree.begin());
  
  Treap<int>::iterator i = tree.find(1);
  EXPECT_EQ(tree.begin(), i);
  EXPECT_EQ(1, *i);
  
  EXPECT_EQ(++i, tree.end());
  EXPECT_EQ(++i, tree.end());

  i = tree.begin();
  EXPECT_EQ(tree.begin(), i++);
  EXPECT_EQ(tree.end(), i++);

  EXPECT_EQ(tree.end(), --tree.end());
  EXPECT_EQ(tree.end(), --tree.begin());

  EXPECT_EQ(1, tree.maximum());
  EXPECT_EQ(1, tree.minimum());

  tree.insert(0);
  tree.insert(2);
  EXPECT_EQ(*tree.begin(), 0);
  EXPECT_EQ(tree.maximum(), 2);

  tree.clear();
}

TEST(SearchTreeTest, TreapPermutations) {
  const int ELEMENTS_COUNT = 5;
  vector<int> elements(ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    elements[i] = i;
  }

  do {
    set<int> correct_tree;
    Treap<int> tree;

    // check insert
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.insert(elements[i]);
      tree.insert(elements[i]);
    
      EXPECT_EQ(tree.size(), i + 1);
    
      // check min/max
      EXPECT_EQ(*correct_tree.begin(), *tree.begin());
      EXPECT_EQ(*correct_tree.begin(), tree.minimum());
      EXPECT_EQ(*(--correct_tree.end()), tree.maximum());

      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (Treap<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }

      EXPECT_EQ(correct_tree_elements, tree_elements);

      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j <= i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }

    // check erase
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
      correct_tree.erase(elements[i]);
      tree.erase(elements[i]);
    
      ASSERT_EQ(ELEMENTS_COUNT - i - 1, tree.size());
      
      // check min/max
      if (i + 1 < ELEMENTS_COUNT) {
        EXPECT_EQ(*correct_tree.begin(), *tree.begin());
        EXPECT_EQ(*correct_tree.begin(), tree.minimum());
        EXPECT_EQ(*(--correct_tree.end()), tree.maximum());
      }
    
      // check all
      vector<int> correct_tree_elements;
      for (set<int>::iterator j = correct_tree.begin();
           j != correct_tree.end();
           ++j) {
        correct_tree_elements.push_back(*j);
      }

      vector<int> tree_elements;
      for (Treap<int>::iterator j = tree.begin();
           j != tree.end();
           ++j) {
        tree_elements.push_back(*j);
      }
    
      EXPECT_EQ(correct_tree_elements, tree_elements);
    
      // find
      for (int j = 0; j < ELEMENTS_COUNT; ++j) {
        if (j > i) {
          EXPECT_FALSE(tree.find(elements[j]) == tree.end());
          EXPECT_EQ(elements[j], *tree.find(elements[j]));
        } else {
          EXPECT_TRUE(tree.find(elements[j]) == tree.end());
        }
      }
    }
  } while (std::next_permutation(elements.begin(), elements.end()));
}

TEST(SearchTreeTest, TreapMonotoneInsert) {
  const int ELEMENTS_COUNT = 100000;
  
  Treap<int> tree;
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(i);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(i, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(ELEMENTS_COUNT - i - 1);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(ELEMENTS_COUNT - i - 1, tree.minimum());
    EXPECT_EQ(ELEMENTS_COUNT - 1, tree.maximum());
  }

  tree.clear();
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    tree.insert(0);
    EXPECT_EQ(i + 1, tree.size());
    EXPECT_EQ(0, tree.minimum());
    EXPECT_EQ(0, tree.maximum());
  }
}
