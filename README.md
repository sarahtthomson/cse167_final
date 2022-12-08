[Meron Asfaw](https://github.com/AdulisL) <br/>
[Sarah Thomson](https://github.com/sarahtthomson)

# Rigid_Body_Simulation

_Welcome to "Our Simulation"! :wave:_
<details id=1 open>
<summary><h2>Step 1: Rigid Bodies</h2></summary>

**What are _Rigid Bodies_?**: 


<details id=2>
<summary><h2>Step 2: Rotation of Rigid Bodies</h2></summary>

_Great job adding the templated workflow! :tada:_

Adding that file to this branch is enough for GitHub Actions to begin running CI on your repository.

When a GitHub Actions workflow is running, you should see some checks in progress, like the screenshot below.

<img alt="checks in progress in a merge box" src=https://user-images.githubusercontent.com/16547949/66080348-ecc5f580-e533-11e9-909e-c213b08790eb.png width=400 />

You can follow along as GitHub Actions runs your job by going to the **Actions** tab or by clicking on "Details" in the merge box below.

When the tests finish, you'll see a red X :x: or a green check mark :heavy_check_mark: in the merge box. At that point, you'll have access to logs for the build job and its associated steps.

<!-- Note here: Learners -- yup, you found the error! Course maintainers -- leave the italics with * instead of _ for the error case. -->

*By looking at the logs, can you identify which tests failed?* To find it, go to one of the failed builds and scrolling through the log. Look for a section that lists all the unit tests. We're looking for the name of the test with an "x".

<img alt="screenshot of a sample build log with the names of the tests blurred out" src=https://user-images.githubusercontent.com/16547949/65922013-e740a200-e3b1-11e9-8151-faf52c30201e.png width=400 />

If the checks don't appear or if the checks are stuck in progress, there's a few things you can do to try and trigger them:

- Refresh the page, it's possible the workflow ran and the page just hasn't been updated with that change.
- Try making a commit on this branch. Our workflow is triggered with a `push` event, and committing to this branch will result in a new `push`.
- Edit the workflow file on GitHub and ensure there are no red lines indicating a syntax problem.

### :keyboard: Activity: Fix the test

1. Update the code in the `ci` branch to get the test to pass. You need to look something like this:
   ```markdown
   _underscore_
   ```
1. **Commit changes**.
1. Wait about 20 seconds then refresh this page for the next step.

</details>

<!--
  <<< Author notes: Step 3 >>>
  Start this step by acknowledging the previous step.
  Define terms and link to docs.github.com.
-->

<details id=3>
<summary><h2>Step 3: Upload test reports</h2></summary>

_The workflow has finished running! :sparkles:_

So what do we do when we need the work product of one job in another? We can use the built-in [artifact storage](https://docs.github.com/en/actions/advanced-guides/storing-workflow-data-as-artifacts) to save artifacts created from one job to be used in another job within the same workflow.

To upload artifacts to the artifact storage, we can use an action built by GitHub: [`actions/upload-artifacts`](https://github.com/actions/upload-artifact).

### :keyboard: Activity: Upload test reports

1. Edit your workflow file.
1. Add a step to your `build` job that uses the `upload-artifacts` action.
   ```yaml
     build:
       runs-on: ubuntu-latest
       steps:
         - uses: actions/checkout@v3

         - name: Run markdown lint
           run: |
             npm install remark-cli remark-preset-lint-consistent
             npx remark . --use remark-preset-lint-consistent --frail

         - uses: actions/upload-artifact@main
           with:
             name: remark-lint-report
             path: public/
   ```
1. Commit your change to this branch.
1. Wait about 20 seconds then refresh this page for the next step.

Similar to the upload action to send artifacts to the storage, you can use another action built by GitHub to download these previously uploaded artifacts from the `build` job: [`actions/download-artifact`](https://github.com/actions/download-artifact). To save you time, we'll skip that step for this course.

</details>

<!--
  <<< Author notes: Step 4 >>>
  Start this step by acknowledging the previous step.
  Define terms and link to docs.github.com.
-->

<details id=4>
<summary><h2>Step 4: Add branch protections</h2></summary>

_Great job uploading test reports! :partying_face:_

Take a look at the merge box, you'll notice you can merge this even though the review process hasn't been met.

Protected branches ensure that collaborators on your repository cannot make irrevocable changes to branches. Enabling protected branches also allows you to enable other optional checks and requirements, like required status checks and required reviews.

### :keyboard: Activity: Add branch protections

1. Go to **Branches** settings. You can navigate to that page manually by clicking on the right-most tab in the top of the repository called **Settings** and then clicking on **Branches**.
1. Click on **Add rule** under "Branch protection rules".
1. Type `main` in **Branch name pattern**.
1. Check **Require pull request reviews before merging**.
1. Check **Require status checks to pass before merging**.
1. Check all build and test jobs that you'd like to see in the newly visible gray box.
1. Click **Create**.
1. _Once you turn on branch protection, Actions can no longer push directly to `main`. You'll need to open the next step on your own._

<!-- Wait about 20 seconds then refresh this page for the next step. -->

</details>

<!--
  <<< Author notes: Step 5 >>>
  Start this step by acknowledging the previous step.
  Define terms and link to docs.github.com.
-->

<details id=5>
<summary><h2>Step 5: Merge your pull request</h2></summary>

_Almost there! :heart:_

You can now [merge](https://docs.github.com/en/get-started/quickstart/github-glossary#merge) your pull request!

### :keyboard: Activity: Merge your pull request

1. Click **Merge pull request**.
1. Delete the branch `ci` (optional).
1. _Once you turn on branch protection, Actions can no longer push directly to `main`. You'll need to open the "finish" on your own._

<!-- Wait about 20 seconds then refresh this page for the next step. -->

</details>

<!--
  <<< Author notes: Finish >>>
  Review what we learned, ask for feedback, provide next steps.
-->

<details id=X>
<summary><h2>Finish</h2></summary>

_Congratulations friend, you've completed this course!_

<img src=https://octodex.github.com/images/Fintechtocat.png alt=celebrate width=300 align=right>

Here's a recap of all the tasks you've accomplished in your repository:

- We created an Actions workflow to lint our Markdown files.
- You caught an issue in a file and fixed the issue before it could make it to `main`.
- You learned how to use build artifacts for test reports.
- You enabled branch protections to require the workflow to pass before merging.

### What's next?

- Get more ideas of what you can do with [awesome actions](https://github.com/sdras/awesome-actions).
- We'd love to hear what you thought of this course [in our discussion board](https://github.com/skills/.github/discussions).
- [Take another GitHub Skills course](https://github.com/skills).
- [Read the GitHub Getting Started docs](https://docs.github.com/en/get-started).
- To find projects to contribute to, check out [GitHub Explore](https://github.com/explore).

</details>

<!--
  <<< Author notes: Footer >>>
  Add a link to get support, GitHub status page, code of conduct, license link.
-->

---

Get help: [Post in our discussion board](https://github.com/skills/.github/discussions) &bull; [Review the GitHub status page](https://www.githubstatus.com/)

&copy; 2022 GitHub &bull; [Code of Conduct](https://www.contributor-covenant.org/version/2/1/code_of_conduct/code_of_conduct.md) &bull; [CC-BY-4.0 License](https://creativecommons.org/licenses/by/4.0/legalcode)
