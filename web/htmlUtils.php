<?php
function htmlHeader ($title) {
   echo <<<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>$title</title>
<link rel="stylesheet" type="text/css" href="mpc.css" />
<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
</head>
<body>
EOF;
}

function htmlFooter () {
   echo "</body>\n</html>";
}

function code ($text) {
   echo '<span class="sc">'.$text.'</span>';
}
?>
