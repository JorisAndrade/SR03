<?php
    session_id($_GET["session"]);
    session_start();
    $_SESSION['hello'] = 'moto';
    print_r($_SESSION);
?>

