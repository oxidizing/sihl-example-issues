open Base;

/* let ok_json_string = {|{"msg":"ok"}|}; */

let ( let* ) = Lwt.bind;

let url = path => "http://localhost:3000/issues" ++ path;

let test_user_creates_board = (_, ()) => {
  let* () = Sihl_core.Manage.clean();
  let* (user, token) =
    Sihl_core.Test.seed @@
    Sihl_users.Seed.logged_in_user(
      ~email="foobar@example.com",
      ~password="123",
    );
  let token = Sihl_users.Model.Token.value(token);
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
      Uri.of_string @@
      url("/users/" ++ Sihl_users.Model.User.id(user) ++ "/boards/"),
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

let test_user_creates_issue_for_board = (_, ()) => Lwt.return();

let test_user_fetches_issues_of_board = (_, ()) => Lwt.return();

let test_user_completes_issue = (_, ()) => Lwt.return();

/* Expect.( */
/*   testPromise("User creates board", () => { */
/*     let%Async (user, {token}) = */
/*       Sihl.App.Main.Manager.seed( */
/*         Sihl.Users.Seeds.loggedInUser("foobar@example.com", "123"), */
/*       ); */
/*     let body = {|{"title": "Board title"}|}; */
/*     let%Async _ = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/boards/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Post, */
/*           ~body=Fetch.BodyInit.make(body), */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */

/*     let%Async boardsResponse = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/users/" ++ Sihl.Users.User.id(user) ++ "/boards/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Get, */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */
/*     let%Async boardsJson = Fetch.Response.json(boardsResponse); */
/*     let boards = */
/*       boardsJson */
/*       |> Routes.GetBoardsByUser.body_out_decode */
/*       |> Belt.Result.getExn; */

/*     let Model.Board.{title} = boards |> Belt.List.headExn; */

/*     title |> expect |> toBe("Board title") |> Sihl.Common.Async.async; */
/*   }) */
/* ); */

/* Expect.( */
/*   testPromise("User creates issue for board", () => { */
/*     let%Async (user, {token}) = */
/*       Sihl.App.Main.Manager.seed( */
/*         Sihl.Users.Seeds.loggedInUser("foobar@example.com", "123"), */
/*       ); */
/*     let%Async board = */
/*       Sihl.App.Main.Manager.seed(Seeds.board(~user, ~title="Board title")); */

/*     let boardId = board.id; */
/*     let body = {j| */
       /*        { */
       /*          "title": "Issue title", */
       /*          "description": "This is the description", */
       /*          "board": "$(boardId)" */
       /*        } */
       /*        |j}; */
/*     let%Async _ = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/issues/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Post, */
/*           ~body=Fetch.BodyInit.make(body), */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */

/*     let%Async issuesResponse = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/boards/" ++ board.id ++ "/issues/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Get, */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */
/*     let%Async issueJson = Fetch.Response.json(issuesResponse); */
/*     let issues = */
/*       issueJson */
/*       |> Routes.GetIssuesByBoard.body_out_decode */
/*       |> Belt.Result.getExn; */

/*     let Model.Issue.{title} = issues |> Belt.List.headExn; */

/*     title |> expect |> toBe("Issue title") |> Sihl.Common.Async.async; */
/*   }) */
/* ); */

/* Expect.( */
/*   testPromise("User fetches issues of board", () => { */
/*     let%Async (user, {token}) = */
/*       Sihl.App.Main.Manager.seed( */
/*         Sihl.Users.Seeds.loggedInUser("foobar@example.com", "123"), */
/*       ); */
/*     let%Async board1 = */
/*       Sihl.App.Main.Manager.seed(Seeds.board(~user, ~title="board 1")); */
/*     let%Async board2 = */
/*       Sihl.App.Main.Manager.seed(Seeds.board(~user, ~title="board 2")); */
/*     let%Async _ = */
/*       Sihl.App.Main.Manager.seed( */
/*         Seeds.issue( */
/*           ~board=board1.id, */
/*           ~user, */
/*           ~title="issue", */
/*           ~description=None, */
/*         ), */
/*       ); */
/*     let%Async issuesResponse = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/boards/" ++ board2.id ++ "/issues/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Get, */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */
/*     let%Async issueJson = Fetch.Response.json(issuesResponse); */

/*     let issues = */
/*       issueJson */
/*       |> Routes.GetIssuesByBoard.body_out_decode */
/*       |> Belt.Result.getExn */
/*       |> Belt.List.toArray; */
/*     issues |> expect |> toHaveLength(0) |> Sihl.Common.Async.async; */
/*   }) */
/* ); */

/* Expect.( */
/*   testPromise("User commpletes issue", () => { */
/*     let%Async (user, {token}) = */
/*       Sihl.App.Main.Manager.seed( */
/*         Sihl.Users.Seeds.loggedInUser("foobar@example.com", "123"), */
/*       ); */
/*     let%Async board = */
/*       Sihl.App.Main.Manager.seed(Seeds.board(~user, ~title="Board title")); */
/*     let%Async issue = */
/*       Sihl.App.Main.Manager.seed( */
/*         Seeds.issue( */
/*           ~board=board.id, */
/*           ~user, */
/*           ~title="Issue title", */
/*           ~description=None, */
/*         ), */
/*       ); */
/*     let%Async _ = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/issues/" ++ issue.id ++ "/complete/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Post, */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */

/*     let%Async issuesResponse = */
/*       Fetch.fetchWithInit( */
/*         baseUrl ++ "/issues/boards/" ++ board.id ++ "/issues/", */
/*         Fetch.RequestInit.make( */
/*           ~method_=Get, */
/*           ~headers= */
/*             Fetch.HeadersInit.make({"authorization": "Bearer " ++ token}), */
/*           (), */
/*         ), */
/*       ); */
/*     let%Async issueJson = Fetch.Response.json(issuesResponse); */
/*     let issues = */
/*       issueJson */
/*       |> Routes.GetIssuesByBoard.body_out_decode */
/*       |> Belt.Result.getExn; */

/*     let Model.Issue.{status} = issues |> Belt.List.headExn; */

/*     status |> expect |> toBe("completed") |> Sihl.Common.Async.async; */
/*   }) */
/* ); */
