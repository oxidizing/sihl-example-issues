let ( let* ) = Lwt.bind;

module Board = {
  let get_all = (request, user) => {
    if (!Sihl_users.Model.User.is_admin(user)) {
      Sihl_core.Fail.raise_no_permissions("Not allowed");
    };
    Repository.Board.get_all |> Sihl_core.Db.query_db_exn(request);
  };

  let get_all_by_user = (request, user, ~user_id) => {
    if (!Sihl_users.Model.User.is_admin(user)
        && Sihl_users.Model.User.is_owner(user, user_id)) {
      Sihl_core.Fail.raise_no_permissions("Not allowed");
    };
    Repository.Board.get_all_by_user(~user_id)
    |> Sihl_core.Db.query_db_exn(request);
  };

  let create = (request, user, ~title) => {
    let board =
      Model.Board.make(~title, ~owner=Sihl_users.Model.User.id(user));
    let* _ =
      Repository.Board.upsert(~board) |> Sihl_core.Db.query_db_exn(request);
    Lwt.return(board);
  };
};

module Issue = {
  let get_all = (request, user) => {
    if (!Sihl_users.Model.User.is_admin(user)) {
      Sihl_core.Fail.raise_no_permissions("Not allowed");
    };
    Repository.Issue.get_all |> Sihl_core.Db.query_db_exn(request);
  };

  let get_all_by_board = (request, user, ~board_id) => {
    let* board =
      Repository.Board.get(~id=board_id)
      |> Sihl_core.Db.query_db_exn(
           ~message="Board not found with that id",
           request,
         );
    if (!Sihl_users.Model.User.is_admin(user)
        && Sihl_users.Model.User.is_owner(user, board.owner)) {
      Sihl_core.Fail.raise_no_permissions("Not allowed");
    };
    Repository.Issue.get_all_by_board(~id=board_id)
    |> Sihl_core.Db.query_db_exn(request);
  };

  let create = (request, user, ~title, ~description, ~board_id) => {
    let* board =
      Repository.Board.get(~id=board_id)
      |> Sihl_core.Db.query_db_exn(
           ~message="Board not found with that id",
           request,
         );
    if (!Sihl_users.Model.User.is_admin(user)
        && Sihl_users.Model.User.is_owner(user, board.owner)) {
      Sihl_core.Fail.raise_no_permissions("Not allowed");
    };
    let issue = Model.Issue.make(~title, ~description, ~board=board_id);
    let* () =
      Repository.Issue.upsert(~issue) |> Sihl_core.Db.query_db_exn(request);
    Lwt.return(issue);
  };

  let complete = (request, user, ~issue_id) => {
    let* issue =
      Repository.Issue.get(~id=issue_id)
      |> Sihl_core.Db.query_db_exn(
           ~message="Issue not found with that id",
           request,
         );
    let* board =
      Repository.Board.get(~id=issue.id)
      |> Sihl_core.Db.query_db_exn(request);
    if (!Sihl_users.Model.User.is_admin(user)
        && Sihl_users.Model.User.is_owner(user, board.owner)) {
      Sihl_core.Fail.raise_no_permissions("Not allowed");
    };
    let issue = Model.Issue.complete(issue);
    Repository.Issue.upsert(~issue) |> Sihl_core.Db.query_db_exn(request);
  };
};
