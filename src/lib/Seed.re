let board = (~user, ~title, request) =>
  Service.Board.create(request, user, ~title);

let issue = (~board_id, ~user, ~title, ~description, request) =>
  Service.Issue.create(request, user, ~title, ~description, ~board_id);
