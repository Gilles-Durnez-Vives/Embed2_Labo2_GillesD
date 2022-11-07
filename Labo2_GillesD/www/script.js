window.addEventListener("load", ()=>{
    fetch("data.json")
        .then((response) => response.json())
        .then((data) => {
            testdiv = document.getElementById("testDiv");
            testdiv.innerText = data["testContent"];
        });
});