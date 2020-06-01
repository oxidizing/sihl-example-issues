open Base;

let ( let* ) = Lwt.bind;

let url = path => "http://localhost:3000/issues" ++ path;

let test_user_fetches_issues_of_board = (_, ()) => {
  let* () = Sihl.Run.Manage.clean();
  let* (user, token) =
    Sihl.Run.Test.seed @@
    Sihl_user.Seed.logged_in_user(
      ~email="foobar@example.com",
      ~password="123",
    );
  let token = Sihl_user.Model.Token.value(token);
  let headers =
    Cohttp.Header.of_list([("authorization", "Bearer " ++ token)]);
  let* board1 =
    Sihl.Run.Test.seed @@
    Sihl_example_issues.Seed.board(~user, ~title="board 1");
  let* board2 =
    Sihl.Run.Test.seed @@
    Sihl_example_issues.Seed.board(~user, ~title="board 2");
  let* _ =
    Sihl.Run.Test.seed @@
    Sihl_example_issues.Seed.issue(
      ~board_id=board1.id,
      ~user,
      ~title="issue",
      ~description=None,
    );
  let* (_, body) =
    Cohttp_lwt_unix.Client.get(
      ~headers,
      Uri.of_string @@ url("/boards/" ++ board2.id ++ "/issues/"),
    );
  let* body = Cohttp_lwt.Body.to_string(body);
  let issues =
    body
    |> Yojson.Safe.from_string
    |> Sihl_example_issues.Handler.GetIssuesByBoard.body_out_of_yojson
    |> Result.ok_or_failwith;
  Alcotest.(check(int))("Has no issues", 0, List.length(issues));
  let* (_, body) =
    Cohttp_lwt_unix.Client.get(
      ~headers,
      Uri.of_string @@ url("/boards/" ++ board1.id ++ "/issues/"),
    );
  let* body = Cohttp_lwt.Body.to_string(body);
  let issues =
    body
    |> Yojson.Safe.from_string
    |> Sihl_example_issues.Handler.GetIssuesByBoard.body_out_of_yojson
    |> Result.ok_or_failwith;
  Lwt.return @@ Alcotest.(check(int))("Has 1 issue", 1, List.length(issues));
};

let test_user_creates_board = (_, ()) => {
  let* () = Sihl.Run.Manage.clean();
  let* (user, token) =
    Sihl.Run.Test.seed @@
    Sihl_user.Seed.logged_in_user(
      ~email="foobar@example.com",
      ~password="123",
    );
  let token = Sihl_user.Model.Token.value(token);
  let headers =
    Cohttp.Header.of_list([("authorization", "Bearer " ++ token)]);
  let body = {json|{"title": "Board title"}|json};
  let* (_, body) =
    Cohttp_lwt_unix.Client.post(
      ~headers,
      ~body=Cohttp_lwt.Body.of_string(body),
      Uri.of_string @@ url("/boards/"),
    );
  let* body = Cohttp_lwt.Body.to_string(body);
  let has_title = String.is_substring(body, ~substring="Board title");
  Alcotest.(check(bool))("Returns ok", true, has_title);
  let* (_, body) =
    Cohttp_lwt_unix.Client.get(
      ~headers,
      Uri.of_string @@ url("/users/" ++ Sihl.User.id(user) ++ "/boards/"),
    );
  let* body = Cohttp_lwt.Body.to_string(body);
  let boards =
    body
    |> Yojson.Safe.from_string
    |> Sihl_example_issues.Handler.GetBoardsByUser.body_out_of_yojson
    |> Result.ok_or_failwith;
  Alcotest.(check(int))("Has one board", 1, List.length(boards));
  Lwt.return();
};

let test_user_creates_issue_for_board = (_, ()) => {
  let* () = Sihl.Run.Manage.clean();
  let* (user, token) =
    Sihl.Run.Test.seed @@
    Sihl_user.Seed.logged_in_user(
      ~email="foobar@example.com",
      ~password="123",
    );
  let* board =
    Sihl.Run.Test.seed @@
    Sihl_example_issues.Seed.board(~user, ~title="Board title");
  let board_id = Sihl_example_issues.Model.Board.id(board);
  let body = [%string
    {json|
{
  "title": "Issue title",
  "description": "This is the description",
  "board": "$(board_id)"
}
|json}
  ];
  let token = Sihl_user.Model.Token.value(token);
  let headers =
    Cohttp.Header.of_list([("authorization", "Bearer " ++ token)]);
  let* _ =
    Cohttp_lwt_unix.Client.post(
      ~headers,
      ~body=Cohttp_lwt.Body.of_string(body),
      Uri.of_string @@ url("/issues/"),
    );
  let* (_, body) =
    Cohttp_lwt_unix.Client.get(
      ~headers,
      Uri.of_string @@ url("/boards/" ++ board_id ++ "/issues/"),
    );
  let* body = Cohttp_lwt.Body.to_string(body);
  let issues =
    body
    |> Yojson.Safe.from_string
    |> Sihl_example_issues.Handler.GetIssuesByBoard.body_out_of_yojson
    |> Result.ok_or_failwith;
  Lwt.return @@
  Alcotest.(check(option(string)))(
    "Created one issue",
    Some("Issue title"),
    issues |> List.hd |> Option.map(~f=Sihl_example_issues.Model.Issue.title),
  );
};

let test_user_completes_issue = (_, ()) => {
  let* () = Sihl.Run.Manage.clean();
  let* (user, token) =
    Sihl.Run.Test.seed @@
    Sihl_user.Seed.logged_in_user(
      ~email="foobar@example.com",
      ~password="123",
    );
  let token = Sihl_user.Model.Token.value(token);
  let headers =
    Cohttp.Header.of_list([("authorization", "Bearer " ++ token)]);
  let* board =
    Sihl.Run.Test.seed @@
    Sihl_example_issues.Seed.board(~user, ~title="board 1");
  let* issue =
    Sihl.Run.Test.seed @@
    Sihl_example_issues.Seed.issue(
      ~board_id=board.id,
      ~user,
      ~title="issue",
      ~description=None,
    );
  let* (_, body) =
    Cohttp_lwt_unix.Client.post(
      ~headers,
      Uri.of_string @@ url("/issues/" ++ issue.id ++ "/complete/"),
    );
  let* body = Cohttp_lwt.Body.to_string(body);
  let issue =
    body
    |> Yojson.Safe.from_string
    |> Sihl_example_issues.Handler.CompleteIssue.body_out_of_yojson
    |> Result.ok_or_failwith;
  Lwt.return @@
  Alcotest.(check(string))("Issue is complete", "completed", issue.status);
};
