(lang dune 2.0)
(generate_opam_files true)

(name sihl_example_issues)
(version 0.0.7)

(authors "Josef Erben")
(license MIT)
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
  (reason (>= 3.6.0))
  (lwt (>= 5.3.0))

  ;; Standard library replacement
  (base (>= v0.13.1))

  ;; Sihl
  (sihl_core (>= 0.0.25))
  (sihl_user (>= 0.0.25))
  (sihl_email (>= 0.0.25))

  ;; Database
  (caqti-driver-postgresql (>= 1.2.1))
  (ppx_rapper (>= 0.9.2))

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
