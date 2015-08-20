class AdalineNN{
    static double alpha;
    double w1, w2, b, err;
    int [][] inp;
    int [] out;
    //constructor of the class AdalineNN which is used to initialize
    //weights and input, output matrix
    public AdalineNN(double w1, double w2, double b, int [][] inp, int [] out) {
        this.w1 = w1;
        this.w2 = w2;
        this.b = b;
        this.inp = inp;
        this.out = out;
        this.err = 0;
    }
    //the function below trains the neural network for desired epochs
    //and prints new calulated weights
    public void trainNN(int epochs) {
        for(int i = 1;i <= epochs;i++) {
            System.out.println("epoch : " + i);
            this.err = 0;
            this.trainNNEpoch();
            this.printNNInfo();
        }
        //after training it checks whether the weights satisfy OR gate
        //condition
        if(this.checkNN())
            System.out.println("final weights satisfy \"OR\" gate");
        else
            System.out.println("final weights don't satisfy \"OR\" gate");
    }
    //the function below trains the neural network for a single epoch
    private void trainNNEpoch() {
        double y, deltaW1, deltaW2, tempErr;
        for(int i = 0;i < 4;i++) {
            y = this.b + inp[0][i] * this.w1 + inp[1][i] * this.w2;
            tempErr = (this.out[i] - y);
            deltaW1 = tempErr * alpha * this.inp[0][i];
            deltaW2 = tempErr * alpha * this.inp[1][i];
            this.w1 += deltaW1;
            this.w2 += deltaW2;
            this.b += (tempErr * alpha);
            this.err += (tempErr * tempErr);
        }
    }
    //activation function used to check the final weights
    private int actFn(double y) {
        if(y >= 0)
            return 1;
        else
            return -1;
    }
    //checks whether final weights comply with OR gate
    private boolean checkNN() {
        int i;
        double y;
        for(i = 0;i < 4;i++) {
            y = this.b + this.inp[0][i] * this.w1 + this.inp[1][i] * this.w2;
            if(this.actFn(y) != this.out[i])
                return false;
        }
        return true;
    }
    //prints info at the end of each epoch
    public void printNNInfo() {
        System.out.println("W1 = " + this.w1 + "\nW2 = " + this.w2 + "\nBias = " + this.b + "\nerr : " + this.err + "\n");
    }
}
public class Adaline {
    public static void main(String [] args) {
        int epochs;
        int [][] inp = {{1, 1, -1, -1}, {1, -1, 1, -1}};
        int [] orOut = {1, 1, 1, -1};
        double w1, w2, b;
        if(args.length != 5) {
            System.out.println("Usage : java Adaline w1 w2 bias alpha epoch");
            return;
        }
        try {
            epochs = Integer.parseInt(args[4]);
        }
        catch(NumberFormatException nfe) {
            System.out.println("Epoch value should be a natural number");
            return;
        }
        if(epochs <= 0) {
            System.out.println("Epoch value should be a natural number");
            return;
        }
        try {
        w1 = Double.parseDouble(args[0]);
        w2 = Double.parseDouble(args[1]);
        b = Double.parseDouble(args[2]);
        AdalineNN.alpha = Double.parseDouble(args[3]);
        }
        catch(NumberFormatException nfe) {
            System.out.println("W1, W2, Bias and Alpha should be real numbers");
            return;
        }
        //creating a neural network for or gate
        AdalineNN orNN = new AdalineNN(w1, w2, b, inp, orOut);
        //initializing with weights and input, output values for training
        //function below starts the training
        orNN.trainNN(epochs);
    }
}
