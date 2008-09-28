<?php
   require_once ("htmlUtils.php");
   require_once ("structureUtils.php");
   htmlHeader ("MPC");
   $menuFile = "menu.xml";
   $content = $_GET ['content'];
   createHeader ();
   createMenu ($menuFile, $content);
   createContent ($menuFile, $content);
   createFooter ($menuFile, $content);
   htmlFooter ();
?>
