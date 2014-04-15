<?php
    session_start();
    $_SESSION['favcolor'] = 'green';
    $_SESSION['animal']   = 'cat';
    $_SESSION['time']     = time();
?>

<input type="button" value="My button" onclick="window.location='display.php?session=<?php echo session_id()?>'"/>
