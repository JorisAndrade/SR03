<?php
$filename = "./martin.jpg";
$desc = fopen($filename, "r");
$content = fread($desc, filesize($filename));
header('Content-Type: image/jpeg');
echo $content;
?>
