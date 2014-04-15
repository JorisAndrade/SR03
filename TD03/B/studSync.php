<?php
    session_start();
?>
<html>
<head>
<meta charset="utf-8">
<style type="text/css">
td {
    border:1px solid black;
}
</style>
</head>
<body>
<?php
    if(isset($_SESSION["last_search"])) {
        echo "<h1> Dernières recherches </h1><table>";
        for($i = 0; $i < count($_SESSION["last_search"]); ++$i) {
            echo "<tr><td>";
            echo $_SESSION["last_search"][($i+$_SESSION["index"])%10];
            echo "</td></tr>";
        }
        echo "</table><br/>";
    }
?>
<form onsubmit="return false">
    <label for="debutnom">debut nom :</label><input id="debutnom" name="debutnom" type="text"> 
    <input type="button" value="envoyer" onClick="displayData()"/>
</form>
<div>
<table>
<thead>
    <tr>
        <td>firstname</td>
        <td>lastname</td>
    </tr>
</thead>
<tbody id="tab_response">
</tbody>
</table>
</div>
</body>
<script>
function displayData() {
    var nom = document.getElementById("debutnom").value;
    var response = document.getElementById("tab_response");
    var xml = getListOfStudents(nom, false);
    response.innerHTML = "";
    var students = xml.getElementsByTagName("utcstudent");
    for (var i = 0; i < students.length; i++) {
        var lastname = students[i].children[0].textContent;
        var firstname = students[i].children[1].textContent;
        response.innerHTML += "<tr><td>" + firstname + "</td><td>" + lastname + "</td></tr>";
    }
}

function getListOfStudents(nomEtu, post) {
  var client = new XMLHttpRequest();
  var baseUrl = "http://tuxa.sme.utc/~sr03/phpinfo/studentdata.php";
  client.open("GET", 'last_search.php?search=' + nomEtu, false);
  client.send();
  if (post) {
    client.open("POST", baseUrl, false);
    client.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    client.send("debutnom=" + nomEtu);
  } else {
    client.open("GET", baseUrl + "?debutnom=" + nomEtu, false);
    client.send();
  }
  return client.responseXML;
}
</script>
</html>
