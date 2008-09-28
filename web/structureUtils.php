<?php

function getEntry ($file, $anchor) {
   // returns the menuIten or subMenuItem associated to the given
   // anchor in the menu file,
   // or the first menuItem if the anchor is not found
   $tab = simplexml_load_file ($file);
   $entry = $tab->menuItem;
   foreach ($tab->menuItem as $item)
      if ($item->anchor == $anchor)
         return $item;
   foreach ($tab->subMenuItem as $item)
      if ($item->anchor == $anchor)
         return $item;
   return $entry;
}

function createHeader () {
   echo '<div id="header">';
   echo '<h1>MPC</h1>';
   echo '</div>';
}

function createMenuItem ($description, $anchor, $disabled) {
   // outputs a menu item for the anchor with the descriptive text
   // if disabled is true, does not create the link
   echo '<li>';
   if (!$disabled) {
      echo '<a href="';
      echo $_SERVER ['PHP_SELF'] . '?content=' . $anchor;
      echo '">';
   }
   echo $description;
   if (!$disabled)
      echo '</a>';
   echo '</li>';
}

function createMenu ($file, $anchor) {
   // takes the name of an xml file describing the menu structure
   // and outputs the menu
   // the anchor corresponds to the current page for which
   // the link is disabled
   if (!isset ($anchor))
      $anchor = getEntry ($file, $anchor)->anchor;
   echo '<div id="menu"><ul>';
   $tab = simplexml_load_file ($file);
   foreach ($tab->menuItem as $item) {
      $disabled = !strcmp ($item->anchor, $anchor);
      createMenuItem ($item->title, $item->anchor, $disabled);
  }
   echo '</ul></div>';
}

function createContent ($file, $anchor) {
   // takes the name of an xml file describing the menu structure
   // and the identifier of the page and outputs its content
   // If $anchor is not set, uses the first menu entry.
   $menuItem = getEntry ($file, $anchor);
   echo '<div id="content">';
   echo '<h1>' . $menuItem->title . '</h1>';
   include ("content_" . $menuItem->anchor . ".php");
   echo '</div>';
}

function createFooter ($file, $anchor) {
   // takes the name of an xml file describing the menu structure
   // and the identifier of the page and outputs the footer
   // If $anchor is not set, uses the first menu entry.
echo <<<EOF
<div id="footer"><hr/>
<address>
Last changed on
EOF;
   echo ' ' . getEntry ($file, $anchor)->date;
echo <<<EOF
 by Andreas Enge
</address>
<a href="http://validator.w3.org/check/referer"><img alt="Valid XHTML 1.0!" src="images/w3c_xhtml10.png"/></a>
<a href="http://jigsaw.w3.org/css-validator/check/referer"><img alt="Valid CSS!" src="images/w3c_css.png"/></a>
</div>
EOF;
}
?>
