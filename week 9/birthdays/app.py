import os

from cs50 import SQL
from flask import Flask, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


def is_valid_date(month, day):
    """Validate date day is valid for month"""

    month_days = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    return (1 <= month-1 <= 12 and 1 <= day <= month_days[month-1])


def validate_form_data(name, month, day):
    """Validate the form data"""

    if not name or not month or not day:
        message = 'Please provide a value for'
        if not name:
            message += ' "name"'
        if not month:
            message += ' "month"'
        if not day:
            message += ' "day"'
        return message

    try:
        month = int(month)
    except ValueError:
        message = 'There was a problem reading "month" value'
        return message
    try:
        day = int(day)
    except ValueError:
        message = 'There was a problem reading "day" value'
        return message

    if not is_valid_date(month, day):
        message = f'Provided date "{month}/{day}" is not valid'
        return message


@app.route("/", methods=["GET", "POST"])
def index():
    """Load index page or add a birthday to the table"""

    if request.method == "POST":

        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        error_message = validate_form_data(name, month, day)
        if error_message:
            return render_template("error.html", message=error_message)

        db.execute(
            "INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    birthday_list = db.execute("SELECT * FROM birthdays")
    return render_template("index.html", birthdays=birthday_list)


@app.route("/delete", methods=["POST"])
def delete_birthday():
    """Delete a birthday from the table"""

    id = request.form.get("id")
    if id:
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")


@app.route("/edit", methods=["POST"])
def edit_birthday():
    """Edit a birthday from the table"""

    id = request.form.get("id")
    name = request.form.get("name")
    month = request.form.get("month")
    day = request.form.get("day")

    error_message = validate_form_data(name, month, day)
    if error_message:
        return render_template("error.html", message=error_message)

    if id:
        db.execute(
            "UPDATE birthdays SET name = ?, month = ?, day = ?  WHERE id = ?", name, month, day, id)
    return redirect("/")
