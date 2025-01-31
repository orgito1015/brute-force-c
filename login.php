<?php
// Predefined credentials
$valid_username = "admin";
$valid_password = "password123";

// Get the submitted form data
$username = $_POST['username'];
$password = $_POST['password'];

// Check if the credentials match
if ($username === $valid_username && $password === $valid_password) {
    echo "<h2>Login successful!</h2>";
    echo "<p>Welcome, " . htmlspecialchars($username) . ".</p>";
} else {
    echo "<h2>Login failed</h2>";
    echo "<p>Incorrect username or password. Please try again.</p>";
}
?>
