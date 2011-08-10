#include "math/simplex.h"

#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <limits>

using std::vector;
using std::pair;
using std::find;
using std::min_element;
using std::swap;
using std::cout;
using std::endl;

void convert_lp_to_standart_form(
    bool is_max,
    const Matrix& A,
    const vector<ConstraintType>& t,
    const Vector& b,
    const Vector& c,
    Matrix& As,
    Vector& bs,
    Vector& cs) {
  As = A;
  bs = b;
  cs = c;

  if (As.empty()) {
    return;
  } else {
    if (cs.empty()) {
      cs = Vector(As[0].size(), 0);
    }
  }

  assert(cs.size() == As[0].size());
  int n = cs.size();
  int m = As.size();

  if (!is_max) {
    for (int i = 0; i < n; ++i) {
      cs[i] = -cs[i];
    }
  }

  for (int k = 0; k < n; ++k) {
    cs.push_back(-cs[k]);
    for (int i = 0; i < m; ++i) {
      As[i].push_back(-As[i][k]);
    }
  }

  for (int i = 0; i < m; ++i) {
    if (t[i] == EQUAL_CONSTRAINT) {
      As.push_back(As[i]);
      bs.push_back(-bs[i]);
      for (int j = 0; j < As.back().size(); ++j) {
        As.back()[j] = -As.back()[j];
      }
    }
  }

  for (int i = 0; i < m; ++i) {
    if (t[i] == GREATER_EQUAL_CONSTRAINT) {
      for (int j = 0; j < As[i].size(); ++j) {
        As[i][j] = -As[i][j];
      }
      bs[i] = -bs[i];
    }
  }
}

void pivot(
    vector<int>& N,
    vector<int>& B,
    Matrix& A,
    Vector& b,
    Vector& c,
    double& v,
    int l, /* outgoing B variable index */
    int e /* ingoing N variable index */) {
  int n = c.size();
  int m = A.size();
  
  double k = A[l][e];
  A[l][e] = 1;
  b[l] /= k;
  for (int j = 0; j < n; ++j) {
    A[l][j] /= k;
  }

  for (int i = 0; i < m; ++i) {
    if (i != l) {
      k = A[i][e];
      A[i][e] = 0;
      b[i] -= k * b[l];
      for (int j = 0; j < n; ++j) {
        A[i][j] -= k * A[l][j];
      }
    }
  }

  v += c[e] * b[l];
  k = c[e];
  c[e] = 0;
  for (int j = 0; j < n; ++j) {
    c[j] -= k * A[l][j];
  }

  swap(B[l], N[e]);
}

// returns whether (A, b, c) is solvable,
// if it is, than (N, B, Ar, br, cr, v) is slack(canonical) form
// with feasible basic solution
// expects that A is not empty
bool initialize_simplex(
    const Matrix& A,
    const Vector& b,
    const Vector& c,
    vector<int>& B,
    vector<int>& N,
    Matrix& Ar,
    Vector& br,
    Vector& cr,
    double& v) {
  int n = c.size();
  int m = A.size();

  Ar = A;
  br = b;
  v = 0;

  for (int i = 0; i < n; ++i) {
    N.push_back(i);
  }
  
  for (int i = n; i < n + m; ++i) {
    B.push_back(i);
  }

  int k = min_element(b.begin(), b.end()) - b.begin();
  if (b[k] >= 0.0) {
    cr = c;
    return true;
  }

  Vector caux(n + 1, 0);
  caux[n] = -1;
  for (int i = 0; i < m; ++i) {
    Ar[i].push_back(-1);
  }
  N.push_back(n + m);

#if 0
  cout << "B:" << endl;
  for (int i = 0; i < B.size(); ++i) {
    cout << B[i] << ' ';
  }
  cout << endl;
  cout << "N:" << endl;
  for (int i = 0; i < N.size(); ++i) {
    cout << N[i] << ' ';
  }
  cout << endl;
  cout << Ar << endl << br << endl << caux << endl << v << endl;
  cout << endl;
#endif

  pivot(N, B, Ar, br, caux, v, k, n);

  for (;;) {
    int e;
    for (e = 0; e < N.size(); ++e) {
      if (caux[e] > EPS) {
        break;
      }
    }
    if (e == N.size()) {
      break;
    }

    double min_ratio = std::numeric_limits<double>::infinity();
    int l = -1;
    for (int i = 0; i < B.size(); ++i) {
      if (Ar[i][e] > 0 && br[i] / Ar[i][e] < min_ratio) {
        min_ratio = br[i] / Ar[i][e];
        l = i;
      }
    }

#if 0
  cout << "B:" << endl;
  for (int i = 0; i < B.size(); ++i) {
    cout << B[i] << ' ';
  }
  cout << endl;
  cout << "N:" << endl;
  for (int i = 0; i < N.size(); ++i) {
    cout << N[i] << ' ';
  }
  cout << endl;
  cout << Ar << endl << br << endl << caux << endl << v << endl;
  cout << "e = " << e << endl;
  cout << "l = " << l << endl;
#endif
    
    assert(l != -1);
    pivot(N, B, Ar, br, caux, v, l, e);
  }
  

  if (v == 0.0) {
    vector<int>::iterator l_iterator = find(B.begin(), B.end(), n + m);
    if (l_iterator != B.end()) {
      int l = l_iterator - B.begin();
      assert(br[l] == 0.0);
      int e;
      for (e = 0; e < N.size(); ++e) {
        if (Ar[l][e] != 0.0) {
          break;
        }
      }
      if (e == N.size()) {
        B.erase(l_iterator);
        Ar.erase(Ar.begin() + l);
        br.erase(br.begin() + l);
      } else {
        pivot(N, B, Ar, br, caux, v, l, e);
      }
    }

    vector<int>::iterator e_iterator = find(N.begin(), N.end(), n + m);
    assert(e_iterator != N.end());
    int e = e_iterator - N.begin();
    for (int i = 0; i < m; ++i) {
      Ar[i].erase(Ar[i].begin() + e);
    }
    N.erase(e_iterator);

    
    cr = Vector(N.size(), 0);
    for (int x = 0; x < n; ++x) {
      vector<int>::iterator n_iterator = find(N.begin(), N.end(), x);
      vector<int>::iterator b_iterator = find(B.begin(), B.end(), x);
      if (b_iterator != B.end()) {
        int l = b_iterator - B.begin();
        for (int j = 0; j < N.size(); ++j) {
          cr[j] -= c[x] * Ar[l][j];
        }
        v += c[x] * br[l];
      } else {
        assert(n_iterator != N.end());
        int e = n_iterator - N.begin();
        cr[e] += c[x];
      }
    }
    return true;
  } else {
    return false;
  }
}
    
void simplex(
    const Matrix& A,
    const Vector& b,
    const Vector& c,
    bool& is_solvable,
    bool& is_limited,
    Vector& x,
    double& v) {
  is_solvable = true;
  is_limited = true;
    
  if (A.empty()) {
    v = 0;
    x = Vector(c.size(), 0);
    for (int i = 0; i < c.size(); ++i) {
      if (c[i] != 0.0) {
        is_limited = false;
        break;
      }
    }
    return;
  }

  vector<int> B, N;
  Matrix Ar;
  Vector br, cr;
  if (!initialize_simplex(A, b, c, B, N, Ar, br, cr, v)) {
    is_solvable = false;
    return;
  }

  
  for (;;) {
    int e;
    assert(N.size() == cr.size());
    for (e = 0; e < N.size(); ++e) {
      if (cr[e] > EPS) {
        break;
      }
    }
    if (e == N.size()) {
      break;
    }

    double min_ratio = std::numeric_limits<double>::infinity();
    int l = -1;
    for (int i = 0; i < B.size(); ++i) {
      if (Ar[i][e] > 0.0 && br[i] / Ar[i][e] < min_ratio) {
        min_ratio = br[i] / Ar[i][e];
        l = i;
      }
    }
    if (l == -1) {
      is_limited = false;
      return;
    } else {
      pivot(N, B, Ar, br, cr, v, l, e);
    }
  }
  
  x.resize(cr.size());
  for (int i = 0; i < cr.size(); ++i) {
    vector<int>::iterator l_iterator = find(B.begin(), B.end(), i); 
    if (l_iterator != B.end()) {
      int l = l_iterator - B.begin();
      x[i] = br[l];
    } else {
      x[i] = 0;
    }
  }
}

void solve_lp(
    bool is_max,
    const Matrix& A,
    const vector<ConstraintType>& t,
    const Vector& b,
    const Vector& c,
    bool& is_solvable,
    bool& is_limited,
    Vector& x,
    double& v) {
  assert(A.size() == b.size());
  assert(A.size() == t.size());

  Matrix As;
  Vector bs, cs;
  convert_lp_to_standart_form(is_max, A, t, b, c, As, bs, cs);
  simplex(As, bs, cs, is_solvable, is_limited, x, v);
  
  if (is_solvable && is_limited) {
    for (int i = 0; i < x.size() / 2; ++i) {
      x[i] -= x[x.size() / 2 + i];
    }
    x.resize(x.size() / 2);
    if (!is_max) {
      v = -v;
    }
  }
}

