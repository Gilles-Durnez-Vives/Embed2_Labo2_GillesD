window.addEventListener("load", ()=>{
    document.cookie = `usernameCGI=defaultUser`;
    fetch("data.json")
    .then((response) => response.json())
    .then((data) => 
    {
        var table = document.getElementById("contentsTable");
        data["Comments"].forEach(element => {
            var newRow = document.createElement("tr");
            var comCel = document.createElement("td");
            var timCel = document.createElement("td");
            var usrCel = document.createElement("td");

            timCel.innerText = element[0];
            usrCel.innerText = element[1];
            comCel.innerText = element[2];
            
            newRow.appendChild(timCel);
            newRow.appendChild(usrCel);
            newRow.appendChild(comCel);

            table.appendChild(newRow);
        });
    });

    document.getElementById("userFormNameInput").addEventListener("focusout", () => {
        if(document.getElementById("userFormNameInput").value != "")
            document.cookie = `usernameCGI=${document.getElementById("userFormNameInput").value};`;
        else
            document.cookie = `usernameCGI=defaultUser`;
        console.log(document.cookie);
    });
});
