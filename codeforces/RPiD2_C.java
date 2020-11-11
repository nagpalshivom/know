import java.util.*;
import java.io.*;

public class RPiD2_C {

    public static PrintWriter outWriter;
    public static MyScanner sc;

    public static void main(String[] args) {
        sc = new MyScanner();
        outWriter = new PrintWriter(new BufferedOutputStream(System.out));

        int n = sc.nextInt();
        long k = sc.nextInt();
        long ans = 0L;

        if(k == 1) {
            Map<Long, Integer> elemCnt = new HashMap<Long, Integer>();
            for(int i = 0; i < n; i++) {
                long a = sc.nextlong();
                elemCnt.put(a, elemCnt.getOrDefault(a, 0) + 1);
            }

            for(Map.Entry<Long, Integer> me : elemCnt.entrySet()) {
                if(me.getValue() > 2) {
                    ans += nc3(me.getValue());
                }
            }
        } else {
            Map<Long, Map<Integer, Long>> m = new HashMap<Long, Map<Integer, Long>>();
            int zeros = 0;

            for(int i = 0; i < n; i++) {
                long a = sc.nextlong();

                if(a == 0L) {
                    zeros++;
                } else {
                    Map<Integer, Long> elemToCntD = m.get(a);
                    if(elemToCntD == null) {
                        elemToCntD = new HashMap<Integer, Long>();
                        m.put(a, elemToCntD);
                    }
                    elemToCntD.put(1, elemToCntD.getOrDefault(1, 0L) + 1L);

                    if(a % k == 0L) {
                        long q = a / k;
                        Map<Integer, Long> elemToCntQ = m.get(q);
                        if(elemToCntQ != null) {
                            for(Map.Entry<Integer, Long> ia : elemToCntQ.entrySet()) {
                                int elems = ia.getKey() + 1;
                                long cnt = elemToCntD.getOrDefault(elems, 0L) + ia.getValue();
                                if(elems == 3) {
                                    ans += ia.getValue();
                                } else if(elems < 3) {
                                    elemToCntD.put(elems, cnt);
                                }
                            }
                        }
                    }
                }
            }

            if(zeros > 2) {
                ans += nc3(zeros);
            }
        }
        outWriter.write("" + ans);
        outWriter.close();
    }

    static long nc3(long a) {
        long t = 1L;
        for(long i = a; i > (a - 3); i--) {
            t *= i;
        }
        t /= 6L;
        return t;
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

        long nextlong() {
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
