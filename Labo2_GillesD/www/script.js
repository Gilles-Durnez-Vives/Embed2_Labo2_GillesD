window.addEventListener("load", ()=>{
    fetch("data.json")
        .then((response) => response.json())
        .then((data) => {
            var table = document.getElementById("contentsTable");
            data["Comments"].forEach(element => {
                var newRow = document.createElement("tr");
                var comCel = document.createElement("td");
                var timCel = document.createElement("td");

                comCel.innerText = element[0];
                timCel.innerText = element[1];

                newRow.appendChild(comCel);
                newRow.appendChild(timCel);

                table.appendChild(newRow);
            });
        });
});