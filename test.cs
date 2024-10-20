using System;

class Solution {
    public static void Main(string[] args) {
        string[] dimensions = Console.ReadLine().Split();
        int N = int.Parse(dimensions[0]);
        int M = int.Parse(dimensions[1]);

        char[,] field = new char[N, M];
        for (int i = 0; i < N; i++) {
            string line = Console.ReadLine();
            for (int j = 0; j < M; j++) {
                field[i, j] = line[j];
            }
        }

        bool?[,] dp = new bool?[N, M];

        for (int i = N - 1; i >= 0; i--) {
            for (int j = M - 1; j >= 0; j--) {
                if (field[i, j] == '*') {
                    dp[i, j] = null;
                } else {
                    bool canWalkRight = i + 1 < N;
                    bool canWalkDown = j + 1 < M;
                    bool canWalk =  canWalkRight || canWalkDown;
                    if (!canWalk) {
                        dp[i, j] = false;
                        continue;
                    }

                    bool isWinnableFromRightSide = canWalkRight && dp[i + 1, j] != null && !dp[i + 1, j].Value;
                    bool isWinnableFromBottomSide = canWalkDown && dp[i, j + 1] != null && !dp[i, j + 1].Value;


                    dp[i, j] = !(isWinnableFromRightSide && isWinnableFromBottomSide);

                }
            }
        }

        Console.WriteLine(dp[0, 0] == true ? 1 : 2);
    }
}
