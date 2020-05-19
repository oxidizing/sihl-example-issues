// TODO remove this and use project definition from Project.re

let config =
  Sihl.Core.Config.Setting.create(
    ~development=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_SENDER", "josef@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
      ("EMAIL_BACKEND", "console"),
    ],
    ~test=[
      ("BASE_URL", "http://localhost:3000"),
      ("EMAIL_SENDER", "josef@oxidizing.io"),
      ("DATABASE_URL", "postgres://admin:password@127.0.0.1:5432/dev"),
      ("EMAIL_BACKEND", "memory"),
    ],
    ~production=[
      ("EMAIL_BACKEND", "smtp"),
      ("BASE_URL", "https://sihl-example-issues.oxidizing.io"),
      ("SMTP_SECURE", "false"),
      ("SMTP_HOST", "smtp.sendgrid.net"),
      ("SMTP_PORT", "587"),
      ("SMTP_AUTH_USERNAME", "apikey"),
    ],
  );

let middlewares = [
  Sihl.Middleware.cookie,
  Sihl.Middleware.static,
  Sihl.Middleware.flash,
  Sihl.Middleware.error,
  Sihl.Middleware.db,
  Sihl_user.Middleware.Authn.token,
  Sihl_user.Middleware.Authn.session,
];

let bindings = [
  Sihl.Core.Registry.bind(
    Sihl.Core.Contract.Migration.repository,
    (module Sihl_repo_postgresql.Repo),
  ),
  Sihl.Core.Registry.bind(
    Sihl_email.Bind.Repository.key,
    (module Sihl_email_repo_postgresql.Repo),
  ),
  Sihl.Core.Registry.bind(
    Sihl_user.Binding.Repository.key,
    (module Sihl_user_repo_postgresql.Repo),
  ),
];

let project =
  Sihl.Run.Project.Project.create(
    ~config,
    ~bindings,
    middlewares,
    [
      (module Sihl_email.App),
      (module Sihl_user.App),
      (module Sihl_example_issues.App),
    ],
  );

let () = Sihl.Run.Project.Project.run_command(project);
