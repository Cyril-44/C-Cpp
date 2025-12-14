#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <vector>

using JMGhI7Alck_ = long long;

constexpr int zR8HHsX = 1e5 + 100;

JMGhI7Alck_ YL48slIvOhH[zR8HHsX];
bool tuorw8HHihO_[zR8HHsX];
JMGhI7Alck_ t0DaqE;

int wHY_J_Zn(int fwE2aA, int g11d9fWyt) { return g11d9fWyt ? wHY_J_Zn(g11d9fWyt, fwE2aA % g11d9fWyt) : fwE2aA; }

void U5nKLZQEq6M(int lzw_fqjY, int ili_m8nWIqJ) {
  int OdC8J6N = wHY_J_Zn(lzw_fqjY, ili_m8nWIqJ);
  int zkVRp_X = ili_m8nWIqJ / OdC8J6N;

  for (int BYXchFo4KCdu = 0; BYXchFo4KCdu < OdC8J6N; BYXchFo4KCdu++) {
    if (tuorw8HHihO_[BYXchFo4KCdu]) continue;
    std::vector<int> FMhsysIGqDp;
    int SYaKKHA = BYXchFo4KCdu;
    for (int OW4fST1qs = 0; OW4fST1qs < zkVRp_X; OW4fST1qs++) {
      FMhsysIGqDp.push_back(SYaKKHA);
      tuorw8HHihO_[SYaKKHA] = true;
      SYaKKHA = (SYaKKHA + lzw_fqjY) % ili_m8nWIqJ;
    }
    for (int Kffyta1hV = 0; Kffyta1hV < 2; Kffyta1hV++) {
      for (int X712P1kiW = 0; X712P1kiW < zkVRp_X; X712P1kiW++) {
        int ikn8cRCtH_ = FMhsysIGqDp[X712P1kiW];
        int ZaR6UVIqx = FMhsysIGqDp[(X712P1kiW + 1) % zkVRp_X];
        if (YL48slIvOhH[ikn8cRCtH_] != LLONG_MAX) {
          // 如果d[las]=LLONG_MAX，d[las]+step会越界，需要特判
          YL48slIvOhH[ZaR6UVIqx] = std::min(YL48slIvOhH[ZaR6UVIqx], YL48slIvOhH[ikn8cRCtH_] + lzw_fqjY);
        }
      }
    }
  }
}

int main() {
  JMGhI7Alck_ RbkCczk4g;
  std::cin >> RbkCczk4g;
  int K_JJwFNeKf[3];
  for (int kmKeesX = 0; kmKeesX < 3; kmKeesX++) {
    std::cin >> K_JJwFNeKf[kmKeesX];
  }
  std::sort(K_JJwFNeKf, K_JJwFNeKf + 3);
  int FuDuaqYeFE = K_JJwFNeKf[0];

  for (int FnPIaQgqHk6F = 0; FnPIaQgqHk6F < FuDuaqYeFE; FnPIaQgqHk6F++) {
    YL48slIvOhH[FnPIaQgqHk6F] = LLONG_MAX;
    // 本题的h达到了ll的上界，如果使用ll的话必需把初值置为LLONG_MAX
  }
  YL48slIvOhH[0] = 0;

  U5nKLZQEq6M(K_JJwFNeKf[1], FuDuaqYeFE);
  memset(tuorw8HHihO_, 0, sizeof(tuorw8HHihO_));
  U5nKLZQEq6M(K_JJwFNeKf[2], FuDuaqYeFE);

  JMGhI7Alck_ JeGqFm = RbkCczk4g - 1;
  t0DaqE = 0;
  for (int vpkOi7 = 0; vpkOi7 < FuDuaqYeFE; vpkOi7++) {
    if (YL48slIvOhH[vpkOi7] <= JeGqFm && YL48slIvOhH[vpkOi7] != LLONG_MAX) {
      t0DaqE += (JeGqFm - YL48slIvOhH[vpkOi7]) / FuDuaqYeFE + 1;
    }
  }
  printf("%lld\n", t0DaqE);
  return 0;
}