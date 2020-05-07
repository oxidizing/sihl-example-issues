// TODO use model from lib
module Issue = {
  [@decco]
  type t = {
    id: string,
    title: string,
    description: option(string),
    board: string,
    assignee: option(string),
    status: string,
  };

  let make = (~title, ~description, ~board) => {
    id: Random.generate(),
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
  [@decco]
  type t = {
    id: string,
    title: string,
    owner: string,
    status: string,
  };

  let make = (~title, ~owner) => {
    id: Random.generate(),
    title,
    owner,
    status: "active",
  };

  let makeId = (~id, ~title, ~owner) => {id, title, owner, status: "active"};
};

module User = {
  [@decco]
  type t = {
    id: string,
    email: string,
    username: option(string),
    password: string,
    status: string,
    admin: bool,
    confirmed: bool,
  };
};
