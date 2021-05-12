package application;

public class MyClass {

    public native void getSchedule(String inFileName, String outFileName, int datastructure , int order , int backfilling);

	public static void main(String[] args) {
        new MyClass().getSchedule("../data/expe/instance_0500_5", "../data/output_EBST", 2 , 1 , 1); // 2 = EBST et 1 = LPT
	}

	static {
		System.loadLibrary("mylibrary");
	}

}
