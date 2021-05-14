package application;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Random;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Line;
import javafx.scene.shape.Polygon;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
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
	@FXML Label ms, wc, wf, wt;
	
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
		fileChooser.setTitle("FileChooser"); 
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

				File resFile = new File("data/res");
			    FileReader fr = new FileReader(resFile);        
			    BufferedReader br = new BufferedReader(fr);    
			    String line;
			    
			    long makespan = 0;
			    long sumWjCj = 0;
			    long sumWjFj = 0;
			    long sumWjTj = 0;
		
			    while((line = br.readLine()) != null) {
			    	String[] tokens = line.split("\t");
			    	// String id = tokens[0];
			    	int p = Integer.parseInt(tokens[1]);
			    	int s = Integer.parseInt(tokens[5]);

			    	makespan = s + p; // on le met à jour à chaque boucle pour qu'en sortant on ait bien le dernier

			    }
			    fr.close();    			    
			    
			    // Construction du diagramme
			    diagramme.getChildren().clear(); // on enlève l'ancien diagramme
			    
			    fr = new FileReader(resFile);        
			    br = new BufferedReader(fr);    
			    
			    while((line = br.readLine()) != null) {
			    	String[] tokens = line.split("\t");
			    	String id = tokens[0];
			    	int p = Integer.parseInt(tokens[1]);
			    	int r = Integer.parseInt(tokens[2]);
			    	int d = Integer.parseInt(tokens[3]);
			    	int w = Integer.parseInt(tokens[4]);
			    	int s = Integer.parseInt(tokens[5]);

			    	int C = s + p;
			    	int F = C - r;
			    	int T = Math.max(0, C - d);
			    	
			    	sumWjCj += (w*C);
			    	sumWjFj += (w*F);
			    	sumWjTj += (w*T);
			    	
			    	int largeur = (int)(((double)p/makespan)*750);
			    	int debut = (int)(((double)s/makespan)*750);
			    	int milieu = (int)( ( (double)(s+( (double)p /2) ) /makespan )*750 );
			    	
			    	// Couleur de la task
			    	Random rand = new Random();
			    	int valeur = 1 + rand.nextInt(5 - 1);
			    	Paint couleur;
			    	switch(valeur) {
			    		case 1 : couleur = Color.web("#A4036F"); // violet
			    			break;
			    		case 2 : couleur = Color.web("#048BA8"); // bleu
			    			break;
			    		case 3 : couleur = Color.web("#16DB93"); // vert
			    			break;
			    		case 4 : couleur = Color.web("#EFEA5A"); // jaune
			    			break;
			    		case 5 : couleur = Color.web("#F29E4C"); // orange
			    			break;
			    		default : couleur = Color.web("#F29E4C"); // orange
			    			break;
			    	}

					Rectangle taskRectangle = new Rectangle(debut,30, largeur, 50); // (<->à gauche, <->au dessus, largeur, hauteur)
					taskRectangle.setFill(couleur);
					taskRectangle.setStroke(Color.BLACK);
					Text taskId = new Text(id);
					taskId.setLayoutX(milieu-7);
					taskId.setLayoutY(60);
					
					Text start = new Text(String.valueOf(s));
					start.setFont(new Font(10));
					start.setLayoutX((int)((double)s/makespan * 750) - 5);
					start.setLayoutY(100);
				
					diagramme.getChildren().addAll(taskRectangle, taskId, start);

			    }
			    
			    fr.close();
			    
			    ms.setText("Makespan = " + makespan);
			    wc.setText("Sum Wj*Cj = " + sumWjCj);
			    wf.setText("Sum Wj*Fj = " + sumWjFj);
			    wt.setText("Sum Wj*Tj = " + sumWjTj);
			    
			    Line trait = new Line(0, 80, 760, 80);
				trait.setStrokeWidth(3);
				Polygon fleche = new Polygon(760, 75, 760, 85, 770, 80);
				
				if (makespan < 200) {
					for (int i = 0 ; i <= makespan ; i++) {
						Line graduation = new Line((int)((double)i/makespan * 750), 80, (int)((double)i/makespan * 750), 86);
						graduation.setStrokeWidth(2);
						
						diagramme.getChildren().addAll(graduation);
					}
				}
				
				Text makespanText = new Text(String.valueOf(makespan));
				makespanText.setFont(new Font(10));
				makespanText.setLayoutX(745);
				makespanText.setLayoutY(100);
				
				diagramme.getChildren().addAll(trait, fleche, makespanText);
			    
				
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
