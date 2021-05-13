package application;

import java.io.File;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import application.MyClass;

enum Order {SPT, LPT, WSPT, FCFS}
enum Struct {OL, BST, EBST}

public class ControllerFenetre {
	@FXML Button chooseFile, exec;
	@FXML ChoiceBox<Order> ordre;
	@FXML ChoiceBox<Struct> structure;
	@FXML CheckBox remplissage;
	@FXML Label fileError;
	@FXML Label fichierChoisiFX;
	@FXML Button btnQuitter;
	@FXML Pane diagramme;
	
	private String fichierChoisi;
	
	public ControllerFenetre() {}
	
	public String getFichierChoisi() {
		return fichierChoisi;
	}

	public void setFichierChoisi(String fichierChoisi) {
		this.fichierChoisi = fichierChoisi;
	}

	@FXML private void initialize() {
		ObservableList<Order> orderChoices = FXCollections.observableArrayList(Order.SPT, Order.LPT, Order.WSPT, Order.FCFS);
		ordre.setItems(orderChoices);
		
		ObservableList<Struct> structChoices = FXCollections.observableArrayList(Struct.OL, Struct.BST, Struct.EBST);
		structure.setItems(structChoices);
		
		ordre.setValue(Order.SPT);
		structure.setValue(Struct.OL);
	}
	
	@FXML
    public void choisirFichier() throws Exception {
		FileChooser fileChooser = new FileChooser();
		fileChooser.setTitle("Exemple de FileChooser"); 
		File homeDir = new File("./data");
		fileChooser.setInitialDirectory(homeDir);
		
		//Affichage et gestion du résultat...
		Stage primaryStage = (Stage) chooseFile.getScene().getWindow();
		File selectedFile = fileChooser.showOpenDialog(primaryStage);
		if(selectedFile!= null) {
			try{
				//---UTILISER ICI LE FICHIER
				setFichierChoisi(selectedFile.getAbsolutePath());
				fichierChoisiFX.setText(selectedFile.getName());
			} catch(Exception e) {
				System.err.println("Erreur: impossible d'utiliser le fichier");
			}
		}
	}
	
	@FXML
    public void executer() throws Exception {
		if (fichierChoisi != null) {
			if (!fichierChoisi.trim().isEmpty()) {
				fileError.setText("");
				int structChoisie = 0;
				int ordreChoisi = 0;
				int backfilling = 0;
				
	
				if (ordre.getValue() == Order.SPT)
					ordreChoisi = 0;
				else if (ordre.getValue() == Order.LPT)
					ordreChoisi = 1;
				else if (ordre.getValue() == Order.WSPT)
					ordreChoisi = 2;
				else if (ordre.getValue() == Order.FCFS)
					ordreChoisi = 3;
				else
					System.err.println("Erreur: ordre");
				// (le switch ne fonctionnait pas)
				
				
				if (structure.getValue() == Struct.OL)
					structChoisie = 0;
				else if (structure.getValue() == Struct.BST)
					structChoisie = 1;
				else if (structure.getValue() == Struct.EBST)
					structChoisie = 2;
				else
					System.err.println("Erreur: structure");
				// (le switch ne fonctionnait pas)

				
				if (remplissage.isSelected())
					backfilling = 1;
				else
					backfilling = 0;
	
				
				// On peut maintenant appeller getSchedule
				new MyClass().getSchedule(fichierChoisi, "data/res", structChoisie, ordreChoisi , backfilling);
				
				// Calcul du makespan et des sommes
				
				
				// Construction du diagramme
				Rectangle taskRectangle = new Rectangle(0,50, 200, 50); // (<->à gauche, <->au dessus, largeur, hauteur)
				taskRectangle.setFill(Color.CORNFLOWERBLUE);
				Text taskId = new Text("01");
				taskId.setLayoutX(80);
				taskId.setLayoutY(80);
				diagramme.getChildren().addAll(taskRectangle, taskId);
				
			}
			else {
				fileError.setText("Attention : pour exécuter le programme vous devez choisir un fichier !");
			}
		}
		else
			fileError.setText("Attention : pour exécuter le programme vous devez choisir un fichier !");
			
	}
	
    @FXML
    public void quitter() throws Exception{
        Stage stage = (Stage) btnQuitter.getScene().getWindow();
        stage.close();
    }
	
}
