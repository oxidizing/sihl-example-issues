module Issue = {
  [@decco]
  [@deriving yojson]
  type t = {
    id: string,
    title: string,
    description: option(string),
    board: string,
    assignee: option(string),
    status: string,
  };

  let make = (~title, ~description, ~board) => {
    id: Sihl_core.Random.uuidv4(),
    title,
    description,
    board,
    assignee: None,
    status: "todo",
  };

  let makeId = (~id, ~title, ~description, ~board) => {
    id,
    title,
    description,
    board,
    assignee: None,
    status: "todo",
  };

  let complete = issue => {...issue, status: "completed"};
  let setStatus = (issue, status) => {...issue, status};
};

module Board = {
  [@deriving yojson]
  [@decco]
  type t = {
    id: string,
    title: string,
    owner: string,
    status: string,
  };

  let make = (~title, ~owner) => {
    id: Sihl_core.Random.uuidv4(),
    title,
    owner,
    status: "active",
  };

  let makeId = (~id, ~title, ~owner) => {id, title, owner, status: "active"};
};
