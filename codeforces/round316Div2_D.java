class round316Div2_D {
    public static void main(String[] args) {
        MyScanner sc = new MyScanner();
        out = new PrintWriter(new BufferedOutputStream(System.out));

        int n, m;
        n = sc.nextInt();
        m = sc.nextInt();
        List<Node> nodes = new ArrayList(n);
        List<Node> levelEndMarkers = new ArrayList(n);


        nodes[0] = new Node(0);
        levelEndMarkers[0] = nodes[0];
        for(int i = 1;i < n; n++) {

            Node nodep = nodes[p - 1];
            nodes[i] = new Node(i);
            Node nodei = nodes[i];

            nodep.children.add(nodei);
            nodei.height = nodep.height + 1;
            Node heightPredecessor = levelEndMarkers[nodei.height];
            if(heightPredecessor != null) {
                nodei.prev = heightPredecessor;
            }
            heightPredecessor = nodei;
        }

        String chars = sc.nextLine();
        for(int i = 0;i < n;i++) {
            nodes[i].c = chars.charAt(i);
        }

        populateMaxHeight(nodes[0]);
        populateLetterCoundTillNow(levelEndMarkers);
        for(int i = 0;i < m;i++) {
            int v = sc.nextInt();
            int h = sc.nextInt();
            Node nodev = nodes[v - 1];
            int l = findLeftmostEdgeVertexAtHeight(nodev);
            int r = findRightmostEdgeVertexAtHeight(nodev);
            int bitForL = nodes[l].letterCountTillNow;
            int bitForR = nodes[r].letterCountTillNow;
            int charAtR = nodes[r].c - 'a';
            int odd = 0;
            for(int j = 0;j < 26 && odd < 2;j++) {
                if(((bitForL & (1 << j)) & ((bitForR | (1 << charAtR)) & (1 << j))) > 0) {
                    odd++
                }
            }
            String ans = "Yes";
            if(odd > 1) {
                ans = "No";
            }
            out.write(ans);
        }

        out.close();
    }

    int findLeftmostEdgeVertexAtHeight(Node root, int height) {
        if(root == null || root.height > height) {
            return -1;
        } else {
            if(root.height == height) {
                return root.v;
            } else {
                int answerFromChild = -1;
                for(Node child : root.children) {
                    answerFromChild = findLeftmostEdgeVertexAtHeight(child, height);
                    if(answerFromChild != -1) {
                        return answerFromChild;
                    }
                }
                return answerFromChild;
            }
        }
    }

    int findRightmostEdgeVertexAtHeight(Node root, int height) {
        if(root == null || root.height > height) {
            return -1;
        } else {
            if(root.height == height) {
                return root.v;
            } else {
                int answerFromChild = -1;
                Iterator descIterator = root.children.descendingIterator();
                while(descIterator.hasNext()) {
                    Node child = descIterator.next();
                    answerFromChild = findRightmostEdgeVertexAtHeight(child, height);
                    if(answerFromChild != -1) {
                        return answerFromChild;
                    }
                }
                return answerFromChild;
            }
        }
    }

    void populateLetterCoundTillNow(List<Node> levelEndMarkers) {
        for(Node levelEndMarker : levelEndMarkers) {
            int letterCountTillNow = 0;
            while(levelEndMarker != null) {
                int bitToSet = levelEndMarker.c - 'a';
                letterCountTillNow = letterCountTillNow | 1 << bitToSet;
                levelEndMarker.letterCountTillNow = letterCountTillNow;
                levelEndMarker = levelEndMarker.prev;
            }
        }
    }

    public int populateMaxHeight(Node root) {
        if(root == null) {
            return 0;
        } else {
            for(Node child : root.children) {
                int prospectiveMaxHeight = getMaxHeight(child) + 1;
                if(root.maxheight < prospectiveMaxHeight) {
                    root.maxheight = prospectiveMaxHeight;
                }
            }
            return root.maxheight;
        }
    }

    public class Node { 
        Node prev;
        char c;
        int v;
        int maxheight;
        int height;
        int letterCountTillNow;
        LinkedList<Node> children = new LinkedList<Node>();

        public Node(int v) {
            this.v = v;
        }
    }

    public static PrintWriter out;
      
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
