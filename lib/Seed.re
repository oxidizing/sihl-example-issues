let board = (~user, ~title, request) =>
  Service.Board.create(user, ~title, request);

let issue = (~board_id, ~user, ~title, ~description, request) =>
  Service.Issue.create(user, ~title, ~description, ~board_id, request);
