(lang dune 2.0)
(generate_opam_files true)

(name sihl_example_issues)
(version 0.0.6)

(authors "Josef Erben")
(license MIT)
(source (uri "git@github.com:oxidizing/sihl-example-issues.git"))
(maintainers "josef@oxidizing.io")
(homepage "https://github.com/oxidizing/sihl-example-issues")
(bug_reports "https://github.com/oxidizing/sihl-example-issues/issues")
(documentation "https://github.com/oxidizing/sihl-example-issues")

(package
 (name sihl_example_issues)
 (synopsis "An Issue Management app using Sihl")
 (description "This project allows users to sign up, login and manage issues and boards of issues. It demonstrates the usage of the Sihl web framework.")
 (license MIT)
 (depends
  ;; General system dependencies
  (dune (>= 2))
  (ocaml (>= 4.08.0))
  (lwt (>= 5.3.0))

  ;; Standard library replacement
  (base (>= v0.13.1))

  ;; Core
  (sihl_core (>= 0.0.19))

  ;; Apps
  (sihl_users (>= 0.0.19))

  ;; Helpers & Generation
  (sexplib (>= 0.13.0))
  (ppx_fields_conv (>= 0.13.0))
  (ppx_sexp_conv (>= 0.13.0))
  (ppx_string_interpolation (>= 1.0.0))

  ;; Dev dependencies
  (utop :dev)
  (merlin :dev)
  (ocamlformat :dev)
  (ocp-indent :dev)
  (tuareg :dev) ;; rm if not using emacs
))
