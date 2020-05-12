let config =
  Sihl_core.Config.Setting.create(
    ~development=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_BACKEND", "console"),
      ("EMAIL_SENDER", "hello@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
    ],
    ~test=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_BACKEND", "memory"),
      ("EMAIL_SENDER", "hello@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
    ],
    ~production=[
      ("BASE_URL", "https://sihl-example-issues.oxidizing.io"),
      ("EMAIL_BACKEND", "sendgrid"),
      ("EMAIL_SENDER", "hello@oxidizing.io"),
      ("SENDGRID_API_KEY", ""),
      ("DATABASE_URL", ""),
    ],
  );

let project =
  Sihl_core.Run.Project.create(
    ~config,
    [
      (module Sihl_email.App),
      (module Sihl_user.App),
      (module Sihl_example_issues.App),
    ],
  );

let () = Sihl_core.Run.Project.run_command(project);
