**Pour lancer sur eclipse (avec e(fx)clipse) :**
- Importer le projet dans eclipse
- Clic droit sur le projet -> build path -> configure build path -> java build path -> libraries -> ajouter JavaFX et JavaFX SDK si ce n’est pas encore fait
- run -> run configurations -> Java Application -> MyClass (càd la classe qui contient le main) -> Arguments -> ajouter dans VM arguments :

```
--module-path "chemin vers le dossier lib du dossier de l'installation de javafx" --add-modules javafx.controls,javafx.fxml
-Djava.library.path=java
```

- puis cliquer sur Apply

**Méthode choisirFichier :**

Suivant l'OS sur lequel est lancé le programme, une erreur pourrait arriver. Lors du choix du fichier, dans la méthode choisirFichier du ControllerFenetre.java à la ligne 68. Il suffit alors de mettre le chemin absolu (selon les conventions de l'OS) du dossier que vous voulez ouvrir par défaut avec le FileChooser.

PS : J'ai laissé mes tests dans le main et 2 instances tests (exemple2 et exemple3) dans le dossier data