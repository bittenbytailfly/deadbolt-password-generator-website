import os
import datetime
import webapp2
from google.appengine.ext.webapp import template

from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app

currentYear = datetime.date.today().year
template_values = {
    'year': currentYear
}


class Index(webapp2.RequestHandler):
    def get(self):
        path = os.path.join(os.path.dirname(__file__), '../views/index.htm')
        self.response.out.write(template.render(path, template_values))


class Licensing(webapp2.RequestHandler):
    def get(self):
        path = os.path.join(os.path.dirname(__file__), '../views/licensing.htm')
        self.response.out.write(template.render(path, template_values))


class RedirectToIndex(webapp2.RequestHandler):
    def get(self):
        self.redirect('/', permanent=True)


class NotFoundPageHandler(webapp2.RequestHandler):
    def get(self):
        self.error(404)
        self.response.out.write('Page not found!')

app = webapp2.WSGIApplication([(r'/', Index),
                               (r'/license-information', Licensing),
                               (r'/faq', RedirectToIndex),
                               (r'/.*', NotFoundPageHandler)],
                               debug=False)