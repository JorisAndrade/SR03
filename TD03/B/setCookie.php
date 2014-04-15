<?php

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $data = $_POST;
} else {
    $data = $_GET;
}  

$name = !empty($data['name']) ? $data['name'] : "cookieName";
$value = !empty($data['value'])? $data['value'] : "cookieValue";
$path = !empty($data['path'])? $data['path'] : "/";
$domain = !empty($data['domain'])? $data['domain'] : $_SERVER['HTTP_HOST'];
$expireseconds = !empty($data['expireseconds'])? $data['expireseconds'] : 0;

setcookie($name, $value, $expireseconds, $path, $domain);


?>
<html>
  <body>
    name : <?php echo $name; ?><br/>
    value : <?php echo $value; ?><br/>
    expires : <?php echo $expireseconds; ?><br/>
    path : <?php echo $path; ?><br/>
    domain : <?php echo $domain; ?><br/>
   </body>
</html>
