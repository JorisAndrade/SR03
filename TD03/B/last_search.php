<?php
    session_start();
    if(!isset($_SESSION["last_search"])) {
        $_SESSION["last_search"] = array();
        $_SESSION["index"] = 0;
    }
    if ($_SESSION["index"] == 10) {
        $_SESSION["index"] = 0;
    }
    $_SESSION["last_search"][$_SESSION["index"]] = $_GET["search"];
    $_SESSION["index"] += 1;
?>
