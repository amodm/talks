// after compilation, use javap -c -p JavaIR
// to inspect the bytecode
public class JavaIR {
    private static int add(int a, int b) {
        return a + b;
    }

    public static void main(String[] args) {
        int a = 10, b = 20;
        int c = JavaIR.add(a, b);
        System.out.println("c = " + c);
    }
}
