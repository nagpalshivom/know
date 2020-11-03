import java.util.*;
import java.io.*;

public class round316Div2_D implements Runnable {

    static Vector<Vector<Integer>> nodeChildren;
    static Vector<Vector<int[]>> heightBasedTimeAndLetterCountPair;
    static int[] in;
    static int[] out;
    static int t;
    static String chars;

    public static PrintWriter outWriter;
    public static MyScanner sc;

    public static void main(String[] args) throws InterruptedException {
        sc = new MyScanner();
        outWriter = new PrintWriter(new BufferedOutputStream(System.out));
        Thread t = new Thread(null, new round316Div2_D(), "round316Div2_D", 1 << 30);
        t.start();
        t.join();
    }

    @Override
    public void run() {

        int n, m;
        n = sc.nextInt();
        m = sc.nextInt();
        nodeChildren = new Vector<Vector<Integer>>(n + 1);
        in = new int[n + 1];
        out = new int[n + 1];
        t = 1;
        for(int i = 0;i <= n;i++) {
            nodeChildren.add(new Vector<Integer>());
        }


        for(int i = 2;i <= n;i++) {
            int p = sc.nextInt();
            nodeChildren.get(p).add(i);
        }
        if(n == 1) {
            sc.nextLine();
        }

        chars = sc.nextLine();

        heightBasedTimeAndLetterCountPair = new Vector<Vector<int[]>>(n + 1);
        for(int i = 0;i < n + 1;i++) {
            heightBasedTimeAndLetterCountPair.add(new Vector<int[]>());
        }
        
        dfs(1, 1);

        for(int i = 0;i < m;i++) {
            String ans = "Yes";
            int v = sc.nextInt();
            int h = sc.nextInt();

            int r;
            int l = getLeftEdge(heightBasedTimeAndLetterCountPair, in[v], out[v], h);
            if(l != -1) {
                r = getRightEdge(heightBasedTimeAndLetterCountPair, in[v], out[v], h);
                if(r != -1) {
                    int ansToCheck = l ^ r;
                    if((ansToCheck & (ansToCheck - 1)) != 0) {
                        ans = "No";
                    }
                }
            }
            outWriter.write(ans + "\n");
        }


        outWriter.close();
    }

    static int getRightEdge(Vector<Vector<int[]>> heightBasedTimeAndLetterCountPair, int in, int out, int h) {
        int probAns = -1;
        Vector<int[]> heightBasedTimeAndLetterCountPairForH = heightBasedTimeAndLetterCountPair.get(h);
        int s = 0;
        int e = heightBasedTimeAndLetterCountPairForH.size() - 1;
        int m = (s + e) / 2;
        while(s <= e && m >= 0) {
            int[] pair = heightBasedTimeAndLetterCountPairForH.get(m);
            if(pair[0] < out && pair[0] > in) {
                s = m + 1;
                probAns = pair[1];
            } else if(pair[0] > out) {
                e = m - 1;
            } else if(pair[0] < in) {
                s = m + 1;
            }
            m = (s + e) / 2;
        }
        return probAns;
    }

    static int getLeftEdge(Vector<Vector<int[]>> heightBasedTimeAndLetterCountPair, int in, int out, int h) {
        int[] probAns = new int[] {0, 0, -1};
        Vector<int[]> heightBasedTimeAndLetterCountPairForH = heightBasedTimeAndLetterCountPair.get(h);
        int s = 0;
        int e = heightBasedTimeAndLetterCountPairForH.size() - 1;
        int m = (s + e) / 2;
        while(s <= e && m >= 0) {
            int[] pair = heightBasedTimeAndLetterCountPair.get(h).get(m);
            if(pair[0] > in && pair[0] < out) {
                e = m - 1;
                probAns = pair;
            } else if(pair[0] < in) {
                s = m + 1;
            } else if(pair[0] > out) {
                e = m - 1;
            }
            m = (s + e) / 2;
        }
        return probAns[1] ^ probAns[2];
    }

    static void dfs(int v, int h) {
        in[v] = t;
        t = t + 2;
        for(int u : nodeChildren.get(v)) {
            dfs(u, h + 1);
        }
        int charBit = 1 << (chars.charAt(v - 1) - 'a');
        int charBitTillNow = charBit;
        if(!heightBasedTimeAndLetterCountPair.get(h).isEmpty()) {
            charBitTillNow = charBit ^ heightBasedTimeAndLetterCountPair.get(h).lastElement()[1];
        }
        heightBasedTimeAndLetterCountPair.get(h).add(new int[] {in[v] + 1, charBitTillNow, charBit});
        out[v] = t++;
    }


    public static class Node { 
        int v;
        Vector<Node> children = new Vector<Node>();
        int in;
        int out;
        int h;

        public Node(int v, int h) {
            this.v = v;
            this.h = h;
        }
    }


    public static class MyScanner {
        BufferedReader br;
        StringTokenizer st;

        public MyScanner() {
            br = new BufferedReader(new InputStreamReader(System.in));
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
