import os
import datetime
from google.appengine.ext.webapp import template

from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app

currentYear = datetime.date.today().year
template_values = {
    'year':currentYear
}

class Index(webapp.RequestHandler):
    def get(self):
        path = os.path.join(os.path.dirname(__file__), '../views/index.htm')
        self.response.out.write(template.render(path, template_values))

class RedirectToIndex(webapp.RequestHandler):
    def get(self):
        self.redirect('/', permanent=True)

class NotFoundPageHandler(webapp.RequestHandler):
    def get(self):
        self.error(404)
        self.response.out.write('Page not found!')

application = webapp.WSGIApplication([
								 ('/', Index), 
                                 (r'/faq', RedirectToIndex),
								 ('/.*', NotFoundPageHandler) 
								 ])

def main():
    run_wsgi_app(application)

if __name__ == "__main__":
    main()