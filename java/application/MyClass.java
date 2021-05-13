package application;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import javafx.scene.Parent;
import javafx.scene.Scene;

public class MyClass extends Application {
	@Override
	public void start(Stage primaryStage) {
		try {
			Parent root = FXMLLoader.load(getClass().getResource("fenetre.fxml"));
			root.getStylesheets().add(getClass().getResource("application.css").toString());
			primaryStage.setTitle("Ordonnancement");
			primaryStage.setScene(new Scene(root, 800, 600));
			primaryStage.show();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}

    public native void getSchedule(String inFileName, String outFileName, int datastructure , int order , int backfilling);

	public static void main(String[] args) {
        launch(args);
	}

	static {
		System.loadLibrary("mylibrary");
	}
	
}
