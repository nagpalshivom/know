class AdalineNN{
    static double alpha = 0.5;
    double w1, w2, b, err;
    int [][] inp;
    int [] out;
    String gate;
    public AdalineNN(double w1, double w2, double b, int [][] inp, int [] out, String gate) {
        this.w1 = w1;
        this.w2 = w2;
        this.b = b;
        this.inp = inp;
        this.out = out;
        this.err = 0;
        this.gate = gate;
    }
    public void trainNN(int epochs) {
        for(int i = 1;i <= epochs;i++) {
            System.out.println("epoch : " + i);
            this.err = 0;
            trainNNEpoch();
            this.printNNInfo();
        }
    }
    private void trainNNEpoch() {
        System.out.println("in train epoch");
        double y, deltaW1, deltaW2, tempErr;
        for(int i = 0;i < 4;i++) {
            y = this.b + inp[0][i] * this.w1 + inp[1][i] * this.w2;
            tempErr = (out[i] - y);
            deltaW1 = tempErr * alpha * inp[0][i];
            deltaW2 = tempErr * alpha * inp[1][i];
            this.w1 += deltaW1;
            this.w2 += deltaW2;
            this.b += (tempErr * alpha);
            err += (tempErr * tempErr);
        }
    }
    private boolean checkNN() {
        int i;
        double y;
        for(i = 0;i < 4;i++) {
            y = b + inp[0][i] * w1 + inp[1][i] * w2;
            if(y != out[i])
                return false;
        }
        return true;
    }
    public void printNNInfo() {
        System.out.println(gate + "-gate\n" + "W1 = " + w1 + "\nW2 = " + w2 + "\nBias = " + b + "\nerr : " + err + "\n");
    }
}
public class Adaline {
    public static void main(String [] args) {
        int [][] inp = {{1, 1, -1, -1}, {1, -1, 1, -1}};
        int [] andOut = {1, -1, -1, -1};
        int [] orOut = {1, 1, 1, -1};
        double w1 = 0.5, w2 = 0.5, b = 0.5;
        //AdalineNN andNN = new AdalineNN(w1, w2, b, inp, andOut, "and");
        AdalineNN orNN = new AdalineNN(w1, w2, b, inp, orOut, "or");
        //andNN.trainNN();
        orNN.trainNN(100);
    }
}
