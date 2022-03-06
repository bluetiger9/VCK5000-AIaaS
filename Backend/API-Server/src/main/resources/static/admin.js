// Server Info components
const serverInfoName = document.querySelector("#server-info-name")
const serverInfoVersion = document.querySelector("#server-info-version")
const serverInfoUpTime = document.querySelector("#server-info-uptime")
const serverInfoHost = document.querySelector("#server-info-host")
const serverInfoUser = document.querySelector("#server-info-user")

// Task List components 
const taskList = document.querySelector("#task-list")

// Task Details components 
const taskDetailsId = document.querySelector("#task-details-id")
const taskDetailsType = document.querySelector("#task-details-type")
const taskDetailsState = document.querySelector("#task-details-state")
const taskDetailsAttributes = document.querySelector("#task-details-attr")
const taskDetailsCreated = document.querySelector("#task-details-created")
const taskDetailsFinished = document.querySelector("#task-details-finished")
const taskDetailsDuration = document.querySelector("#task-details-duration")
const taskDetailsDetails = document.querySelector("#task-details-details")

// server base
const serverBase = window.location.origin + "/admin"

// Fetch Server Info
async function fetchServerInfo() {
    const url = new URL(serverBase + '/server-info');    

    // remote call
    const response = await fetch(url);
    if (response.status != 200) {
        // error response
        throw await response.text();
    }

    // success response
    return await response.json();
}

// Fetch Task list
async function fetchTaskList() {
    const url = new URL(serverBase + '/tasks');

    // remote call
    const response = await fetch(url);
    if (response.status != 200) {
        // error response
        throw await response.text();
    }

    // success response
    return await response.json();
}

// Fetch Task info
async function fetchTaskDetails(id) {
    const url = new URL(serverBase + '/tasks/' + id);

    // remote call
    const response = await fetch(url);
    if (response.status != 200) {
        // error response
        throw await response.text();
    }

    // success response
    return await response.json();
}

async function fetchAndRenderServerInfo() {
    serverInfo = await fetchServerInfo()
    serverInfoName.innerHTML = serverInfo.name
    serverInfoVersion.innerHTML = serverInfo.version
    serverInfoUpTime.innerHTML = serverInfo.upTime + " sec"
    serverInfoHost.innerHTML = serverInfo.hostname
    serverInfoUser.innerHTML = serverInfo.username
}

async function fetchAndRenderTaskList() {
    var taskListResp = await fetchTaskList()

    var oldRows = taskList.querySelectorAll(".task-list-row")
    if (oldRows) {
        oldRows.forEach(row => taskList.removeChild(row))
    }

    taskListResp.forEach(task => {
        var row = document.createElement('tr');
        row.classList.add("task-list-row");
        row.innerHTML = ""
            + "<td>" + task.id + "</td>"
            + "<td>" + task.type + "</td>"
            + "<td>" + task.state + "</td>"
            + "<td>" + JSON.stringify(task.attributes) + "</td>"
            + "<td>" + task.creationDate + "</td>"
            + "<td>" + task.finishedDate + "</td>"
            + "<td>" + task.duration + " sec</td>";
        taskList.appendChild(row);

        row.onclick = function() {
            oldSelected = taskList.querySelectorAll(".task-list-selected")
            if (oldSelected) {
                oldSelected.forEach(row => row.classList.remove("task-list-selected"));
            }
        
            row.classList.add("task-list-selected")

            fetchAndRenderTaskDetails(task.id)
        }
    });
}

async function fetchAndRenderTaskDetails(id) {
    taskDetailsResp = await fetchTaskDetails(id)

    taskDetailsId.innerHTML = taskDetailsResp.id
    taskDetailsType.innerHTML = taskDetailsResp.type
    taskDetailsState.innerHTML = taskDetailsResp.state
    taskDetailsAttributes.innerHTML = JSON.stringify(taskDetailsResp.attributes)
    taskDetailsCreated.innerHTML = taskDetailsResp.creationDate
    taskDetailsFinished.innerHTML = taskDetailsResp.finishedDate
    taskDetailsDuration.innerHTML = taskDetailsResp.duration + " sec"
    taskDetailsDetails.innerHTML =  JSON.stringify(taskDetailsResp.details)
}

fetchAndRenderServerInfo();
fetchAndRenderTaskList();
