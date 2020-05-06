open Alcotest_lwt;

let ( let* ) = Lwt.bind;

let suite = [
  (
    "test",
    [
      test_case(
        "user creates board",
        `Quick,
        Test_issues.test_user_creates_board,
      ),
      test_case(
        "user creates issue for board",
        `Quick,
        Test_issues.test_user_creates_issue_for_board,
      ),
      test_case(
        "user fetches issues of board",
        `Quick,
        Test_issues.test_user_fetches_issues_of_board,
      ),
      test_case(
        "user completes issue",
        `Quick,
        Test_issues.test_user_completes_issue,
      ),
    ],
  ),
];

let () =
  Lwt_main.run(
    {
      let* () = Sihl_core.Manage.start(Run.project);
      let* () = Sihl_core.Manage.migrate();
      let* () = run("issue management app", suite);
      Sihl_core.Manage.stop();
    },
  );
