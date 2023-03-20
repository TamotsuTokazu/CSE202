#include <bits/stdc++.h>

using U = unsigned long long;

const U Z = 0x11u;
const U L = 3 * Z;
const U I = 5 * Z;
const U T = 7 * Z;
const U C[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

struct State {
    U x, l, r;

    bool operator<(const State &a) const {
        if (x == a.x) {
            return l == a.l ? r < a.r : l < a.l;
        } else {
            return x < a.x;
        }
    }

    int findR(int pos) const {
        int t = 1;
        while (1) {
            t -= (r >> pos) & 1;
            if (t == 0) {
                return pos;
            }
            pos++;
            t += (l >> pos) & 1;
        }
    }

    int findL(int pos) const {
        int t = 1;
        while (1) {
            t -= (l >> pos) & 1;
            if (t == 0) {
                return pos;
            }
            pos--;
            t += (r >> pos) & 1;
        }
    }

    void del(int pos) {
        if (~(x >> pos) & 1) {
            return;
        }
        if ((l >> pos) & ~(r >> pos) & 1) { // <
            int t = 0, q = pos + 1;
            while (1) {
                t += (l >> q) & 1;
                if (t == 0 && ((x >> q) & 1)) {
                    l ^= ((U)1 << pos) | ((U)1 << q);
                    break;
                }
                t -= (r >> q) & 1;
                q++;
            }
        } else if (~(l >> pos) & (r >> pos) & 1) { // >
            int t = 0, q = pos - 1;
            while (1) {
                t += (r >> q) & 1;
                if (t == 0 && ((x >> q) & 1)) {
                    r ^= ((U)1 << pos) | ((U)1 << q);
                    break;
                }
                t -= (l >> q) & 1;
                q--;
            }
        } else {
            l ^= l & ((U)1 << pos);
            r ^= r & ((U)1 << pos);
        }
        x ^= (U)1 << pos;
    }

    void ins(int lp, int rp) {
        x |= ((U)1 << lp) | ((U)1 << rp);
        l |= ((U)1 << lp);
        r |= ((U)1 << rp);
    }

    bool join(int pos) { // join pos and pos + 1
        if ((r >> pos) & (l >> (pos + 1)) & 1) {
            r ^= ((U)1 << pos);
            l ^= ((U)1 << (pos + 1));
        } else if ((r >> pos) & 1) {
            int lp = findL(pos);
            r ^= ((U)1 << pos);
            l ^= ((U)1 << lp);
        } else if ((l >> (pos + 1)) & 1) {
            int rp = findR(pos + 1);
            l ^= ((U)1 << (pos + 1));
            r ^= ((U)1 << rp);
        } else {
            return false;
        }
        return true;
    }
    
    void prettyPrint() const {
        for (int i = 0; i < 8 * sizeof(U); i++) {
            printf("%d", (x >> i) & 1);
        }
        puts("");
        for (int i = 0; i < 8 * sizeof(U); i++) {
            printf("%d", (l >> i) & 1);
        }
        puts("");
        for (int i = 0; i < 8 * sizeof(U); i++) {
            printf("%d", (r >> i) & 1);
        }
        puts("\n");
    }
} ini = {0, 0, 0};

using SS = std::set<State>;

void expand(SS &z, State x, int p, U ss, int q) {
    for (int i = 0; i < q; i++) {
        auto s = (ss >> i) & 15;
        if ((((x.x >> p) ^ s) & 3) == 0) {
            auto u = s & 3;
            State y = x;
            // lots of cases :(
            switch (ss) {
            case Z:
                if (u == 0) { // create
                    if (s == 8) { // p
                        y.ins(p, p);
                    } else { // p + 1
                        y.ins(p + 1, p + 1);
                    }
                } else { // delete
                    if (s == 1) { // p
                        y.del(p);
                    } else { // p + 1
                        y.del(p + 1);
                    }
                }
                z.insert(y);
                break;

            case L:
                if (u == 3) { // join p, p + 1
                    if (!y.join(p)){
                        break;
                    }
                    y.del(p);
                    y.del(p + 1);
                } else if (u == 0) { // create new at p, p + 1
                    y.ins(p, p + 1);
                } // else remain the same
                z.insert(y);
                break;

            case I:
                // swap p + 1
                y.x ^= (U)3 << p;
                if ((y.l | (y.l >> 1)) >> p & 1) {
                    y.l ^= (U)3 << p;
                }
                if ((y.r | (y.r >> 1)) >> p & 1) {
                    y.r ^= (U)3 << p;
                }
                z.insert(y);
                break;

            case T:
                if (u == 3) { // first join
                    if (!y.join(p)) {
                        break;
                    }
                    if (s & 8) { // keep p, del p + 1
                        y.del(p + 1);
                    } else {
                        y.del(p);
                    }
                } else { // first create
                    if (u == 1) {
                        y.ins(p + 1, p + 1);
                    } else {
                        y.ins(p, p);
                    }
                    y.join(p);
                }
                z.insert(y);
                break;

            default:
                break;
            }
        }
    }
}

char board[101][101];
int n = 0, m = 0;

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%s", board[i]);
    }
    SS s0 = {ini};
    for (int i = 0; i < n; i++) {
        SS s;
        for (const auto &i : s0) {
            s.insert({i.x << 1, i.l << 1, i.r << 1});
        }
        for (int j = 0; j < m; j++) {
            SS t;
            for (auto &x : s) {
                switch (board[i][j]) {
                case 'Z':
                    expand(t, x, j, Z, 4);
                    break;
                
                case 'L':
                    expand(t, x, j, L, 4);
                    break;

                case 'I':
                    expand(t, x, j, I, 2);
                    break;

                case 'T':
                    expand(t, x, j, T, 4);
                    break;
                
                default:
                    expand(t, x, j, 0, 1);
                    break;
                }
            }
            std::swap(s, t);
        }
        s0.clear();
        for (auto &x : s) {
            if (x.x >> m == 0) {
                s0.insert(x);
            }
        }
        printf("%d\t%d\n", i, s0.size());
    }
    puts("s0");
    printf("%d\n", s0.size());
    for (auto x : s0) {
        x.prettyPrint();
    }
    return 0;
}
