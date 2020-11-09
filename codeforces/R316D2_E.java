import java.util.*;
import java.io.*;

public class R316D2_E {

    public static PrintWriter outWriter;
    public static MyScanner sc;
    public static Long MOD = 1000000007L;

    public static void main(String[] args) {
        sc = new MyScanner();
        outWriter = new PrintWriter(new BufferedOutputStream(System.out));
        int n = sc.nextInt();
        int m = sc.nextInt();

        String[] p = new String[n];

        for(int i = 0;i < n;i++) {
            p[i] = sc.nextLine();
        }

        int midDist = (n + m - 2) / 2;

        long[][][] dp = new long[2][n + 2][n + 2];

        if(p[0].charAt(0) == p[n - 1].charAt(m - 1)) {
            dp[0][1][n] = 1L;
        }

        long ans = 0L;
        int curr = 0;

        for(int dist = 1; dist <= midDist; dist++) {
            for(int x1 = 1; x1 <= n; x1++) {
                for(int x2 = 1; x2 <= n; x2++) {
                    int y1 = dist + 2 - x1;
                    int y2 = n + m - x2 - dist;

                    if(y1 > 0 && y1 <= m && y2 > 0 && y2 <= m && y1 <= y2 && x1 <= x2 
                        && p[x1 - 1].charAt(y1 - 1) == p[x2 - 1].charAt(y2 - 1)) {
                        dp[1 - curr][x1][x2] = madd(dp[curr][x1 - 1][x2], 
                            madd(dp[curr][x1 - 1][x2 + 1], 
                                madd(dp[curr][x1][x2], dp[curr][x1][x2 + 1])));
                    } else {
                        dp[1 - curr][x1][x2] = 0l;
                    }
                }
            }
            curr = 1 - curr;
        }

        for(int x1 = 1; x1 <= n; x1++) {
            for(int x2 = 1; x2 <= n; x2++) {
                if(dp[curr][x1][x2] > 0) {
                    ans = madd(ans, dp[curr][x1][x2]);
                }
            }
        }

        outWriter.write("" + ans);
        outWriter.close();
    }

    static long madd(long a, long b) {
        return (long)(a + b) % MOD;
    }

    public static class MyScanner {
        BufferedReader br;
        StringTokenizer st;

        public MyScanner() {
            br = new BufferedReader(new InputStreamReader(System.in));
            outWriter = new PrintWriter(new BufferedOutputStream(System.out));
        }

        String next() {
            while (st == null || !st.hasMoreElements()) {
                try {
                    st = new StringTokenizer(br.readLine());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return st.nextToken();
        }

        int nextInt() {
            return Integer.parseInt(next());
        }

        long nextLong() {
            return Long.parseLong(next());
        }

        double nextDouble() {
            return Double.parseDouble(next());
        }

        String nextLine(){
            String str = "";
            try {
                str = br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return str;
        }

    }


}
