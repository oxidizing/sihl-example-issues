let ( let* ) = Lwt.bind;

module Board = {
  let get_all = (request, user) => {
    if (!Sihl_user.Model.User.is_admin(user)) {
      Sihl.Core.Err.raise_no_permissions("Not allowed");
    };
    Repository.Board.get_all |> Sihl.Core.Db.query_db_exn(request);
  };

  let get_all_by_user = (request, user, ~user_id) => {
    if (!Sihl_user.Model.User.is_admin(user)
        && !Sihl_user.Model.User.is_owner(user, user_id)) {
      Sihl.Core.Err.raise_no_permissions("Not allowed");
    };
    Repository.Board.get_all_by_user(~user_id)
    |> Sihl.Core.Db.query_db_exn(request);
  };

  let create = (request, user, ~title) => {
    let board =
      Model.Board.make(~title, ~owner=Sihl_user.Model.User.id(user));
    let* _ =
      Repository.Board.upsert(~board) |> Sihl.Core.Db.query_db_exn(request);
    Lwt.return(board);
  };
};

module Issue = {
  let get_all = (request, user) => {
    if (!Sihl_user.Model.User.is_admin(user)) {
      Sihl.Core.Err.raise_no_permissions("Not allowed");
    };
    Repository.Issue.get_all |> Sihl.Core.Db.query_db_exn(request);
  };

  let get_all_by_board = (request, user, ~board_id) => {
    let* board =
      Repository.Board.get(~id=board_id)
      |> Sihl.Core.Db.query_db_exn(
           ~message="Board not found with that id",
           request,
         );
    if (!Sihl_user.Model.User.is_admin(user)
        && !Sihl_user.Model.User.is_owner(user, board.owner)) {
      Sihl.Core.Err.raise_no_permissions("Not allowed");
    };
    Repository.Issue.get_all_by_board(~id=board_id)
    |> Sihl.Core.Db.query_db_exn(request);
  };

  let create = (request, user, ~title, ~description, ~board_id) => {
    let* board =
      Repository.Board.get(~id=board_id)
      |> Sihl.Core.Db.query_db_exn(
           ~message="Board not found with that id",
           request,
         );
    if (!Sihl_user.Model.User.is_admin(user)
        && !Sihl_user.Model.User.is_owner(user, board.owner)) {
      Sihl.Core.Err.raise_no_permissions("Not allowed");
    };
    let issue = Model.Issue.make(~title, ~description, ~board=board_id);
    let* () =
      Repository.Issue.upsert(~issue) |> Sihl.Core.Db.query_db_exn(request);
    Lwt.return(issue);
  };

  let complete = (request, user, ~issue_id) => {
    let* issue =
      Repository.Issue.get(~id=issue_id)
      |> Sihl.Core.Db.query_db_exn(
           ~message="Issue not found with that id",
           request,
         );
    let* board =
      Repository.Board.get(~id=issue.board)
      |> Sihl.Core.Db.query_db_exn(request);
    if (!Sihl_user.Model.User.is_admin(user)
        && !Sihl_user.Model.User.is_owner(user, board.owner)) {
      Sihl.Core.Err.raise_no_permissions("Not allowed");
    };
    let issue = Model.Issue.complete(issue);
    let* () =
      Repository.Issue.upsert(~issue) |> Sihl.Core.Db.query_db_exn(request);
    Lwt.return(issue);
  };
};
